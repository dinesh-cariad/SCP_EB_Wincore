[!/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('XCP_VARS_M'))"!][!/*
*/!][!VAR "XCP_VARS_M"="'true'"!][!/*

*** Number of predefined DAQ lists ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
  */!][!VAR "XcpMinDaq" = "num:i(XcpGeneral/XcpMinDaq)"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "XcpMinDaq" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Variable to check that master block mode for programming is supported ***
*/!][!VAR "XcpMasterBlockModePgmSupported" = "'false'"!][!/*
*/!][!IF "(XcpGeneral/XcpPgmSupported = 'true') and node:exists(XcpGeneral/XcpMasterBlockModePgmSupported) and (XcpGeneral/XcpMasterBlockModePgmSupported = 'true')"!][!/*
  */!][!VAR "XcpMasterBlockModePgmSupported" = "'true'"!][!/*
*/!][!ENDIF!][!/*

*** Variable to hold whether direction STIM is supported ***
*/!][!VAR "XcpStimSupported" = "'false'"!][!/*
*/!][!IF "(XcpGeneral/XcpDaqSupported = 'true') and node:exists(XcpGeneral/XcpStimSupported) and (XcpGeneral/XcpStimSupported = 'true')"!][!/*
  */!][!VAR "XcpStimSupported" = "'true'"!][!/*
*/!][!ENDIF!][!/*

*/!][!IF "node:exists(XcpGeneral/XcpDaqConfigType)"!][!/*
  */!][!VAR "XcpDaqConfigType" = "XcpGeneral/XcpDaqConfigType"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "XcpDaqConfigType" = "''"!][!/*
*/!][!ENDIF!][!/*

*** Minimum size of an  ODT Entry. Must be updated when Xcp Supports Granularity other than BYTE.  ***
*/!][!VAR "XcpMinODTEntrySize" = "1"!][!/*

*** Length of the identification field (PID) ***
*/!][!VAR "XcpPidSize" = "0"!][!/*
*/!][!IF "XcpGeneral/XcpIdentificationFieldType = 'ABSOLUTE'"!][!/*
  */!][!VAR "XcpPidSize" = "1"!][!/*
*/!][!ELSEIF "XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_BYTE'"!][!/*
  */!][!VAR "XcpPidSize" = "2"!][!/*
*/!][!ELSEIF "XcpGeneral/XcpIdentificationFieldType = 'RELATIVE_WORD'"!][!/*
  */!][!VAR "XcpPidSize" = "3"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "XcpPidSize" = "4"!][!/*
*/!][!ENDIF!][!/*

*/!][!IF "(count(XcpConfig/*/XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']) > 0)"!][!/*
  */!][!IF "(count(XcpConfig/*/XcpConnectionCfg/*[XcpConnectionInterfaceType/XcpCanFdMaxDlcRequired = 'true']) > 0)"!][!/*
    *** Variable to calculate the Maximum Data Length for CANFD ***
    */!][!VAR "XcpCanFdMaxDataLength" ="0"!][!/*
    */!][!VAR "TempXcpCanFdMaxDataLength" ="0"!][!/*
    */!][!LOOP "XcpConfig/*/XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverCANFD']"!][!/*
      */!][!VAR "XcpCanFdMaxDataLength" = "XcpConnectionInterfaceType/XcpCanFdMaxDlc"!][!/*
      */!][!IF "$XcpCanFdMaxDataLength > $TempXcpCanFdMaxDataLength"!][!/*
        */!][!VAR "TempXcpCanFdMaxDataLength" =  "$XcpCanFdMaxDataLength"!][!/*
      */!][!ELSE!][!/*
        */!][!VAR "XcpCanFdMaxDataLength" = "$TempXcpCanFdMaxDataLength"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*

*** Number of predefined ODTs ***
  */!][!VAR "XcpNoOfPreODTs" = "num:i(count(XcpConfig/*[1]/XcpDaqList/*[XcpDaqListNumber < $XcpMinDaq]/XcpOdt/*))"!][!/*

*** Number of predefined ODT Entries ***
  */!][!VAR "XcpNoOfPreODTEntries" = "num:i(count(XcpConfig/*[1]/XcpDaqList/*[XcpDaqListNumber < $XcpMinDaq]/XcpOdt/*/XcpOdtEntry/*))"!][!/*

*** Number of Dynamic Daq Lists ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true' and $XcpDaqConfigType = 'DAQ_DYNAMIC'"!][!/*
  */!][!VAR "XcpDaqCount" = "num:i(XcpGeneral/XcpDaqCount)"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "XcpDaqCount" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Total number of DAQ lists ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
 */!][!IF "($XcpDaqConfigType = 'DAQ_STATIC')"!][!/*
   */!][!VAR "XcpMaxDaq" = "num:integer(count(XcpConfig/*[1]/XcpDaqList/*))"!][!/*
 */!][!ELSE!][!/*
   */!][!VAR "XcpMaxDaq" = "$XcpDaqCount + $XcpMinDaq"!][!/*
 */!][!ENDIF!][!/*
*/!][!ELSE!][!/*
 */!][!VAR "XcpMaxDaq" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Total number of predefined/static DAQ lists ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
 */!][!IF "($XcpDaqConfigType = 'DAQ_STATIC')"!][!/*
   */!][!VAR "XcpNoOfNonDynamicDaqLists" = "$XcpMaxDaq"!][!/*
 */!][!ELSE!][!/*
   */!][!VAR "XcpNoOfNonDynamicDaqLists" = "$XcpMinDaq"!][!/*
 */!][!ENDIF!][!/*
*/!][!ELSE!][!/*
 */!][!VAR "XcpNoOfNonDynamicDaqLists" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Number of ODTs allowed for a Dynamic Daq List ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true' and $XcpDaqConfigType = 'DAQ_DYNAMIC'"!][!/*
  */!][!VAR "XcpOdtCount" = "num:i(XcpGeneral/XcpOdtCount)"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "XcpOdtCount" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Number of static ODTs ***
*/!][!VAR "XcpNoOfStaODTs" = "0"!][!/*
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
  */!][!IF "$XcpDaqConfigType = 'DAQ_STATIC'"!][!/*
    */!][!IF "(($XcpMaxDaq - $XcpMinDaq) >0)"!][!/*
      */!][!LOOP "XcpConfig/*[1]/XcpDaqList/*[XcpDaqListNumber >= $XcpMinDaq]"!][!/*
        */!][!VAR "XcpNoOfStaODTs" ="num:i($XcpNoOfStaODTs + XcpMaxOdt)"!][!/*
     */!][!ENDLOOP!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*

*** Total number of possible ODTs ***
*/!][!VAR "XcpTotalNumberOfOdts" = "0"!][!/*
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
  */!][!IF "$XcpDaqConfigType = 'DAQ_STATIC'"!][!/*
    */!][!VAR "XcpTotalNumberOfOdts" = "num:i($XcpNoOfPreODTs + $XcpNoOfStaODTs)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "XcpTotalNumberOfOdts" = "num:i(($XcpDaqCount * $XcpOdtCount) + $XcpNoOfPreODTs)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*

*** Total number of predefined and/or static ODTs ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
 */!][!IF "($XcpDaqConfigType = 'DAQ_STATIC')"!][!/*
   */!][!VAR "XcpNoOfNonDynamicODTs" = "$XcpTotalNumberOfOdts"!][!/*
 */!][!ELSE!][!/*
   */!][!VAR "XcpNoOfNonDynamicODTs" = "$XcpNoOfPreODTs"!][!/*
 */!][!ENDIF!][!/*
*/!][!ELSE!][!/*
 */!][!VAR "XcpNoOfNonDynamicODTs" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Total number of Event channels ***
*/!][!IF "node:exists(XcpConfig/*[1]/XcpEventChannel/*)"!][!/*
  */!][!VAR "XcpMaxEventChannel" = "count(XcpConfig/*[1]/XcpEventChannel/*)"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "XcpMaxEventChannel" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Boolean to check if timestamp is configured or not ***
*/!][!VAR "XcpTimestampIsEnabled" = "(XcpGeneral/XcpTimestampType != 'NO_TIME_STAMP') and (XcpGeneral/XcpDaqSupported = 'true')"!][!/*

*** Variable to check if storage of DAQ list in NV memory is supported ***
*/!][!VAR "StoreDaq" = "'false'"!][!/*
*/!][!IF "node:exists(XcpGeneral/XcpStoreDaqSupported)"!][!/*
  */!][!VAR "StoreDaq" = "XcpGeneral/XcpStoreDaqSupported"!][!/*
*/!][!ENDIF!][!/*

*** The timestamp size ***
*/!][!VAR "XcpTimestampSize" = "num:i((number(XcpGeneral/XcpTimestampType ='NO_TIME_STAMP') * 0) + (number(XcpGeneral/XcpTimestampType ='ONE_BYTE') * 1) + (number(XcpGeneral/XcpTimestampType ='TWO_BYTE') * 2) + (number(XcpGeneral/XcpTimestampType ='FOUR_BYTE') * 4))"!][!/*

*** Calculate the free ODT entry space for DAQ lists with timestamp enabled ***
*/!][!VAR "XcpAbsoluteMaxOdtEntrySize" = "num:i((num:min(XcpConfig/*[1]/XcpConnectionCfg/*/XcpMaxDto) - ($XcpTimestampSize + $XcpPidSize)) div $XcpMinODTEntrySize)"!][!/*

*** Number of static ODT Entries  ***
*/!][!VAR "XcpNoOfStaODTEntries" = "0"!][!/*
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
  */!][!IF "$XcpDaqConfigType = 'DAQ_STATIC'"!][!/*
    */!][!IF "(($XcpMaxDaq - $XcpMinDaq) >0)"!][!/*
      */!][!LOOP "XcpConfig/*[1]/XcpDaqList/*[XcpDaqListNumber >= $XcpMinDaq]"!][!/*
        */!][!IF "(XcpMaxOdtEntries >= $XcpAbsoluteMaxOdtEntrySize) and ($XcpTimestampIsEnabled = 'true')"!][!/*
          */!][!VAR "XcpNoOfStaODTEntries" ="num:i($XcpNoOfStaODTEntries + (XcpMaxOdt * XcpMaxOdtEntries)) - $XcpTimestampSize"!][!/*
        */!][!ELSE!][!/*
          */!][!VAR "XcpNoOfStaODTEntries" ="num:i($XcpNoOfStaODTEntries + (XcpMaxOdt * XcpMaxOdtEntries))"!][!/*
        */!][!ENDIF!][!/*
     */!][!ENDLOOP!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*

*** Total number of predefined and/or static ODT Entries ***
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
 */!][!IF "($XcpDaqConfigType = 'DAQ_STATIC')"!][!/*
   */!][!VAR "XcpNoOfNonDynamicODTEntries" = "num:i($XcpNoOfPreODTEntries + $XcpNoOfStaODTEntries)"!][!/*
 */!][!ELSE!][!/*
   */!][!VAR "XcpNoOfNonDynamicODTEntries" = "$XcpNoOfPreODTEntries"!][!/*
 */!][!ENDIF!][!/*
*/!][!ELSE!][!/*
 */!][!VAR "XcpNoOfNonDynamicODTEntries" = "0"!][!/*
*/!][!ENDIF!][!/*

*** Variable to calculate the number of bits for the Bus Timeout Counter's data type ***
*/!][!IF "num:i(round(num:f(num:div(XcpGeneral/XcpTxBusTimeout, XcpGeneral/XcpMainFunctionPeriod)))) < bit:shl(1,8)"!][!/*
  */!][!VAR "BusToTypeSize" = "num:i(8)"!][!/*
*/!][!ELSEIF "num:i(round(num:f(num:div(XcpGeneral/XcpTxBusTimeout, XcpGeneral/XcpMainFunctionPeriod)))) < bit:shl(1,16)"!][!/*
  */!][!VAR "BusToTypeSize" = "num:i(16)"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "BusToTypeSize" = "num:i(32)"!][!/*
*/!][!ENDIF!][!/*

*** Variable to calculate the number of bits for the FlexRay hardware buffer timeout counter data type ***
*/!][!VAR "XcpFlexRaySyncTimeoutCycles" = "num:i(round(num:f(num:div(XcpGeneral/XcpFlexRaySyncTimeout, XcpGeneral/XcpMainFunctionPeriod))))"!][!/*
*/!][!IF "$XcpFlexRaySyncTimeoutCycles < bit:shl(1,8)"!][!/*
  */!][!VAR "FlexRayToTypeSize" = "num:i(8)"!][!/*
*/!][!ELSEIF "$XcpFlexRaySyncTimeoutCycles < bit:shl(1,16)"!][!/*
  */!][!VAR "FlexRayToTypeSize" = "num:i(16)"!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "FlexRayToTypeSize" = "num:i(32)"!][!/*
*/!][!ENDIF!][!/*

*** Variable to enable the packing/unpaking multiple PDUs in one frame ***
*/!][!VAR "MultiplePdusInOneFrame" = "(count(XcpConfig/*[1]/XcpConnectionCfg/*[(node:exists(XcpConnectionInterfaceType/XcpPackMultiMsgInOneFlexRayFrame) and (XcpConnectionInterfaceType/XcpPackMultiMsgInOneFlexRayFrame = 'true')) or (node:exists(XcpConnectionInterfaceType/XcpPackMultiMsgInOneEthernetFrame) and (XcpConnectionInterfaceType/XcpPackMultiMsgInOneEthernetFrame = 'true'))]) > 0)"!][!/*

*** Variable to enable the FlexRay sequence correction ***
*/!][!VAR "FlexRaySequenceCorrection" = "(count(XcpConfig/*[1]/XcpConnectionCfg/*[(node:exists(XcpConnectionInterfaceType/XcpSequenceCorrectionEnabled) and (XcpConnectionInterfaceType/XcpSequenceCorrectionEnabled = 'true'))]) > 0)"!][!/*

*** Variable holding the number of configured connections ***
*/!][!VAR "NoOfConnections" = "count(XcpConfig/*[1]/XcpConnectionCfg/*)"!][!/*

*** Variable to enable the automaticaly open socket connections ***
*/!][!VAR "AutomaticOpenSoCon" = "(count(XcpConfig/*[1]/XcpConnectionCfg/*[node:exists(XcpConnectionInterfaceType/XcpAutoOpenSoCon) and (XcpConnectionInterfaceType/XcpAutoOpenSoCon = 'true')]) > 0)"!][!/*

*** Variable to enable the support for SoAd Socket Protocol Tcp ***
*/!][!VAR "SuportForSoAdSocketProtocolTcp" = "'false'"!][!/*
*/!][!VAR "NoOfTcpIpConnections" = "0"!][!/*
*/!][!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*[node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)]"!][!/*
  */!][!VAR "TxPduName"="node:ref(node:ref(XcpTxPduConnectionInfo/*[1]/XcpMapTxPdu2Connection)/XcpTxPduRef)/@name"!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!][!/*
    */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]"!][!/*
      */!][!VAR "SuportForSoAdSocketProtocolTcp" = "'true'"!][!/*
      */!][!VAR "NoOfTcpIpConnections" = "$NoOfTcpIpConnections + 1"!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!][!/*
    */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]"!][!/*
      */!][!VAR "SuportForSoAdSocketProtocolTcp" = "'true'"!][!/*
      */!][!VAR "NoOfTcpIpConnections" = "$NoOfTcpIpConnections + 1"!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*** Variable to enable the support for SoAd Socket Protocol Udp ***
*/!][!VAR "SuportForSoAdSocketProtocolUdp" = "'false'"!][!/*
*/!][!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*[node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)]"!][!/*
  */!][!VAR "TxPduName"="node:ref(node:ref(XcpTxPduConnectionInfo/*[1]/XcpMapTxPdu2Connection)/XcpTxPduRef)/@name"!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!][!/*
    */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp')]"!][!/*
      */!][!VAR "SuportForSoAdSocketProtocolUdp" = "'true'"!][!/*
      */!][!BREAK!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!][!/*
    */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp')]"!][!/*
      */!][!VAR "SuportForSoAdSocketProtocolUdp" = "'true'"!][!/*
      */!][!BREAK!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*** Variable to verify if there is at least one IPv4 Udp connection ***
*/!][!VAR "IsUpdIPv4" = "'false'"!][!/*
*/!][!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*[node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)]"!][!/*
  */!][!VAR "TxPduName"="node:ref(node:ref(XcpTxPduConnectionInfo/*[1]/XcpMapTxPdu2Connection)/XcpTxPduRef)/@name"!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)"!][!/*
    */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp') and node:refvalid(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketLocalAddressRef) and (node:ref(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnectionRef)/../../SoAdSocketLocalAddressRef)/TcpIpDomainType = 'TCPIP_AF_INET')]"!][!/*
      */!][!VAR "IsUpdIPv4" = "'true'"!][!/*
      */!][!BREAK!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!][!/*
    */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp') and node:refvalid(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketLocalAddressRef) and (node:ref(node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketLocalAddressRef)/TcpIpDomainType  = 'TCPIP_AF_INET')]"!][!/*
      */!][!VAR "IsUpdIPv4" = "'true'"!][!/*
      */!][!BREAK!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*** Variable to store size of predefined lists + size of static lists ***
*/!][!VAR "NonDynamicMirrorSize" = "0"!][!/*
*** Variable to store maximum size of an ODT ***
*/!][!VAR "MaxSizeOdt" = "0"!][!/*
*** Variable to store maximum size of an ODT Entry ***
*/!][!VAR "MaxSizeOdtEntry" = "0"!][!/*

*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*

  */!][!SELECT "XcpGeneral"!][!/*
    */!][!VAR "MaxSizeOdtEntry" = "XcpOdtEntrySizeDaq"!][!/*
    */!][!IF "(XcpStimSupported = 'true') and (XcpOdtEntrySizeStim > $MaxSizeOdtEntry)"!][!/*
      */!][!VAR "MaxSizeOdtEntry" = "XcpOdtEntrySizeStim"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*

  */!][!SELECT "XcpConfig/*[1]"!][!/*
    */!][!LOOP "node:order(XcpDaqList/*, 'XcpDaqListNumber')"!][!/*
        */!][!IF "XcpDaqListNumber < $XcpMinDaq"!][!/*
          */!][!LOOP "node:order(XcpOdt/*, 'XcpOdtNumber')"!][!/*
            */!][!VAR "SizeOdt" = "0"!][!/*
            */!][!LOOP "node:order(XcpOdtEntry/*, 'XcpOdtEntryNumber')"!][!/*
              */!][!VAR "NonDynamicMirrorSize" = "$NonDynamicMirrorSize + XcpOdtEntryLength"!][!/*
              */!][!VAR "SizeOdt" = "$SizeOdt + XcpOdtEntryLength"!][!/*
            */!][!ENDLOOP!][!/*
            */!][!IF "$MaxSizeOdt < $SizeOdt"!][!/*
              */!][!VAR "MaxSizeOdt" = "$SizeOdt"!][!/*
            */!][!ENDIF!][!/*
          */!][!ENDLOOP!][!/*
        */!][!ELSE!][!/*
          */!][!VAR "NonDynamicMirrorSize" = "$NonDynamicMirrorSize + (XcpMaxOdt * XcpMaxOdtEntries * $MaxSizeOdtEntry)"!][!/*
          */!][!IF "$MaxSizeOdt < XcpMaxOdtEntries * $MaxSizeOdtEntry"!][!/*
            */!][!VAR "MaxSizeOdt" = "XcpMaxOdtEntries * $MaxSizeOdtEntry"!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
  */!][!ENDSELECT!][!/*

  */!][!SELECT "XcpGeneral"!][!/*
    */!][!IF "(XcpDaqConfigType = 'DAQ_DYNAMIC') and ($MaxSizeOdt < XcpOdtEntriesCount * $MaxSizeOdtEntry)"!][!/*
        */!][!VAR "MaxSizeOdt" = "XcpOdtEntriesCount * $MaxSizeOdtEntry"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDIF!][!/*

*** Variable to store maximum ODT number in a list***
*/!][!VAR "MaxOdtInList" = "0"!][!/*
*/!][!IF "XcpGeneral/XcpDaqSupported = 'true'"!][!/*
  */!][!SELECT "XcpConfig/*[1]"!][!/*
    */!][!LOOP "node:order(XcpDaqList/*, 'XcpDaqListNumber')"!][!/*
        */!][!IF "XcpDaqListNumber < $XcpMinDaq"!][!/*
          */!][!VAR "OdtInList" = "num:i(count(XcpOdt/*))"!][!/*
          */!][!IF "$MaxOdtInList < $OdtInList"!][!/*
            */!][!VAR "MaxOdtInList" = "$OdtInList"!][!/*
          */!][!ENDIF!][!/*
        */!][!ELSE!][!/*
           */!][!IF "$MaxOdtInList < XcpMaxOdt"!][!/*
            */!][!VAR "MaxOdtInList" = "XcpMaxOdt"!][!/*
          */!][!ENDIF!][!/*
        */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
  */!][!ENDSELECT!][!/*
  */!][!SELECT "XcpGeneral"!][!/*
    */!][!IF "(XcpDaqConfigType = 'DAQ_DYNAMIC')"!][!/*
      */!][!IF "$MaxOdtInList <  XcpOdtCount"!][!/*
        */!][!VAR "MaxOdtInList" = "XcpOdtCount"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDIF!][!/*

*/!][!SELECT "XcpConfig/*[1]"!][!/*

*************************************************************************
* Local variable used to calculate the number of cyclic event channels processed by the MainFunction
*************************************************************************
*/!][!VAR "CyclicEventCount"="count(XcpEventChannel/*[(XcpEventChannelTimeCycle > 0) and (../../../../XcpGeneral/XcpEventTriggerMainFunc = 'false')])"!][!/*

************************************************
* Local variables: time unit constants
************************************************
*/!][!VAR "Event_Channel_Time_Unit_1ns"   = "0.000000001"!][!/*      1ns   = 0.000000001s
*/!][!VAR "Event_Channel_Time_Unit_10ns"  = "0.00000001"!][!/*       10ns  = 0.00000001s
*/!][!VAR "Event_Channel_Time_Unit_100ns" = "0.0000001"!][!/*        100ns = 0.0000001s
*/!][!VAR "Event_Channel_Time_Unit_1us"   = "0.000001"!][!/*         1us   = 0.000001s
*/!][!VAR "Event_Channel_Time_Unit_10us"  = "0.00001"!][!/*          10us  = 0.00001s
*/!][!VAR "Event_Channel_Time_Unit_100us" = "0.0001"!][!/*           100us = 0.0001s
*/!][!VAR "Event_Channel_Time_Unit_1ms"   = "0.001"!][!/*            1ms   = 0.001s
*/!][!VAR "Event_Channel_Time_Unit_10ms"  = "0.01"!][!/*             10ms  = 0.01s
*/!][!VAR "Event_Channel_Time_Unit_100ms" = "0.1"!][!/*              100ms = 0.1s
*/!][!VAR "Event_Channel_Time_Unit_1s"    = "1.0"!][!/*              1s    = 1s
*/!][!VAR "Event_Channel_Time_Unit_1ps"   = "0.000000000001"!][!/*   1ps   = 0.000000000001s
*/!][!VAR "Event_Channel_Time_Unit_10ps"  = "0.00000000001"!][!/*    10ps  = 0.00000000001s
*/!][!VAR "Event_Channel_Time_Unit_100ps" = "0.0000000001"!][!/*     100ps = 0.0000000001s

*/!][!/* Local variable: XcpMainFunctionPeriod*/!][!/*
*/!][!VAR "XcpMainFuncPeriod" = "../../XcpGeneral/XcpMainFunctionPeriod"!][!/*

*/!][!IF "../../XcpGeneral/XcpPgmSupported = 'true'"!][!/*
*/!][!VAR "XcpMaxOfMaxCtoPgm"="num:max(XcpConnectionCfg/*/XcpMaxCtoPgm)"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "XcpMaxOfMaxCtoPgm"="num:i(0)"!][!/*
*/!][!ENDIF!][!/*

************************************************
* Macro: EVENT_CHANNEL_TIME_CYCLE_COUNTER_MAX
************************************************
*/!][!MACRO "EVENT_CHANNEL_TIME_CYCLE_COUNTER_MAX", "time_unit" = "'TIMESTAMP_UNIT_1MS'"!][!/*
  */!][!IF "$time_unit = 'TIMESTAMP_UNIT_1NS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1ns div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10NS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10ns div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100NS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100ns div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1US'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1us div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10US'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10us div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100US'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100us div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1MS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1ms div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10MS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10ms div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100MS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100ms div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1S'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1s div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1PS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1ps div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10PS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10ps div $XcpMainFuncPeriod))"!][!/*
  */!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100PS'"!][!/*
  */!]    [!"num:i(round(XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100ps div $XcpMainFuncPeriod))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*/!][!ENDSELECT!][!/*

*** Macro: EVENT_CHANNEL_PERIOD***
*/!][!MACRO "EVENT_CHANNEL_PERIOD", "time_unit" = "'TIMESTAMP_UNIT_1MS'"!][!/*
*/!][!IF "$time_unit = 'TIMESTAMP_UNIT_1NS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1ns"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10NS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10ns"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100NS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100ns"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1US'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1us"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10US'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10us"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100US'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100us"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1MS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1ms"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10MS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10ms"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100MS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100ms"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1S'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1s"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_1PS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_1ps"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_10PS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_10ps"!][!/*
*/!][!ELSEIF "$time_unit = 'TIMESTAMP_UNIT_100PS'"!][!/*
*/!][!"XcpEventChannelTimeCycle * $Event_Channel_Time_Unit_100ps"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*************************************************************************
* MACRO to get the string for the symbolic name value.
* "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
* "OldName" must hold an unique string for the case that the specification
* conform short name does not exist. This is supplied to be backward compatible.
*************************************************************************
*/!][!MACRO "GetSymbolName","ShortNameRef","OldName"!][!/*
  */!][!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!/*
  */!][!IF "$SymbolName = ''"!][!/*
    */!][!VAR "SymbolName" = "concat(node:name($ShortNameRef),'_',$OldName)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "SymbolName" = "concat(node:name($ShortNameRef),'_',$SymbolName)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*


*************************************************************************
* MACRO to get the pdu size.
* "XcpMaxDataLength" the maximum value between the connection specific XcpMaxCto, XcpMaxCtoPgm and XcpMaxDto
* "PackMultiMsgInOneFrame" used to decide if the packing of multiple PDUs in one frame is allowed
* "MaxMsgLengthInit" configured value for XcpMaxMsgLength[Flex/Ethernet]Init parameter
* "XcpConnectionType" the type of the selected Xcp connection
* "FlxHeaderAlignment" configured value for XcpFlxHeaderAlignment parameter
*************************************************************************
*/!][!MACRO "GetPackagePduSize","XcpMaxDataLength","PackMultiMsgInOneFrame","MaxMsgLengthInit","XcpConnectionType","FlxHeaderAlignment","SequenceCorrection"!][!/*
  */!][!VAR "XcpPackageHeaderSize" = "0"!][!/*
  */!][!IF "$XcpConnectionType = 'XcpConnectionOverFlexRay'"!][!/*
    */!][!VAR "XcpPackageAlignment" = "node:when($FlxHeaderAlignment = 'PACKET_ALIGNMENT_8', 1, node:when($FlxHeaderAlignment = 'PACKET_ALIGNMENT_16', 2, node:when($FlxHeaderAlignment = 'PACKET_ALIGNMENT_32', 4, 0)))"!][!/*
    */!][!IF "$PackMultiMsgInOneFrame = 'true'"!][!/*
          */!][!IF "$SequenceCorrection = 'true'"!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_8'"!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "3"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "4"!][!/*
              */!][!ENDIF!][!/*
          */!][!ELSE!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_32'"!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "4"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "2"!][!/*
              */!][!ENDIF!][!/*
          */!][!ENDIF!][!/*
    */!][!ELSE!][!/*
          */!][!IF "$SequenceCorrection = 'true'"!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_32'"!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "4"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "2"!][!/*
              */!][!ENDIF!][!/*
          */!][!ELSE!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_32'"!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "4"!][!/*
              */!][!ELSEIF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_16'"!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "2"!][!/*
              */!][!ELSEIF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_8'"!][!/*
                 */!][!VAR "XcpPackageHeaderSize" = "1"!][!/*
              */!][!ENDIF!][!/*
          */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "XcpPackagePduSize" = "node:when(num:f(num:mod($XcpPackageHeaderSize + $XcpMaxDataLength, 2.0)) = 0, $XcpPackageHeaderSize + $XcpMaxDataLength, $XcpPackageHeaderSize + $XcpMaxDataLength + 1)"!][!/*
  */!][!ELSEIF "$XcpConnectionType = 'XcpConnectionOverEthernet'"!][!/*
    */!][!VAR "XcpPackageHeaderSize" = "4"!][!/*
    */!][!VAR "XcpPackagePduSize" = "$XcpPackageHeaderSize + $XcpMaxDataLength"!][!/*
  */!][!ELSEIF "$XcpConnectionType = 'XcpConnectionOverCANFD'"!][!/*
    */!][!IF "(count(XcpConfig/*/XcpConnectionCfg/*[XcpConnectionInterfaceType/XcpCanFdMaxDlcRequired = 'true']) > 0)"!][!/*
      */!][!VAR "XcpPackagePduSize" = "$XcpCanFdMaxDataLength"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "XcpPackagePduSize" = "$XcpMaxDataLength"!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "XcpPackagePduSize" = "$XcpMaxDataLength"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "$PackMultiMsgInOneFrame = 'true'  and ($FlxAssignEnabled = 'false')"!][!/*
    */!][!VAR "XcpPackagePduSize" = "$MaxMsgLengthInit"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*************************************************************************
* MACRO to get the Tx Pdu info.
* Input parameters:
** "ConnectionPath" the XPath of the selected connection node(XcpConnectionCfg)
** "TxPduName" EcuC's short name for the selected Xcp Tx Pdu
** "TxPduId" Xcp Tx Pdu internal ID
* Output parameters:
** "ComIf" Return the Transport layer comunication type (Values: CanIf, FrIf and SoAd)
** "ComIfPduDestId" return The Tx Pdu destination Id from ComIf
** "CanTxId" returns:
**           * the CAN Id, if available,
**           * empty string, otherwise.
** "XcpPackagePduSize" The Xcp Pdu size
** "TxFlags" return the type of packages supported by the selected Tx Pdu(CTO, EvCTO, DaqDTO)
*************************************************************************
*/!][!MACRO "GetTxPduInfo","ConnectionPath","TxPduName","TxPduId","TxPduCddTriggerTransmit", "TxPduConnectionInfoPath"=""!][!/*
  */!][!VAR "ComIf" = "''"!][!/*
  */!][!VAR "CanTxId"="''"!][!/*
  */!][!VAR "ComIfPduDestId" = "''"!][!/*
  */!][!VAR "TxFlags" = "''"!][!/*
  */!][!SELECT "node:ref($ConnectionPath)"!][!/*
    */!][!VAR "PduMapIndex"="0"!][!/*
    */!][!VAR "FlxHeaderAlignment"="''"!][!/*
    */!][!VAR "SequenceCorrection"="'false'"!][!/*
    */!][!VAR "PackMultiMsgInOneFrame"="'false'"!][!/*
    */!][!VAR "MaxMsgLengthInit"="0"!][!/*
    */!][!LOOP "node:order(XcpTxPduConnectionInfo/*, 'node:ref(XcpMapTxPdu2Connection)/XcpTxPduId')"!][!/*
      */!][!IF "node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId"!][!/*
        */!][!BREAK!][!/* we found the position
      */!][!ENDIF!][!/*
      */!][!VAR "PduMapIndex"="$PduMapIndex + 1"!][!/*
    */!][!ENDLOOP!][!/*
****************************************************************
* Get Pdu Destination Id (transport layer interface)
****************************************************************
    */!][!IF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionCanIfCfgRef)"!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionCanIfCfgRef)/CanIfTxPduCfg/*[node:refvalid(CanIfTxPduRef) and (node:ref(CanIfTxPduRef)/@name = $TxPduName) and ((text:toupper(CanIfTxPduUserTxConfirmationUL) = 'XCP') or (text:toupper(CanIfTxPduUserTxConfirmationUL) = 'CDD'))]"!][!/*
        */!][!VAR "ComIf" = "'CanIf'"!][!/*
        */!][!VAR "ComIfPduDestId" = "CanIfTxPduId"!][!/*
        */!][!IF "CanIfTxPduCanIdType = 'EXTENDED_CAN'"!][!/*
          */!][!VAR "CanTxId" = "bit:bitset(num:i(CanIfTxPduCanId),31)"!][!/*
        */!][!ELSE!][!/*
          */!][!VAR "CanTxId" = "CanIfTxPduCanId"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ELSEIF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionCanFdCanIfCfgRef)"!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionCanFdCanIfCfgRef)/CanIfTxPduCfg/*[node:refvalid(CanIfTxPduRef) and (node:ref(CanIfTxPduRef)/@name = $TxPduName) and ((text:toupper(CanIfTxPduUserTxConfirmationUL) = 'XCP') or (text:toupper(CanIfTxPduUserTxConfirmationUL) = 'CDD'))]"!][!/*
        */!][!VAR "ComIf" = "'CanIf'"!][!/*
        */!][!VAR "ComIfPduDestId" = "CanIfTxPduId"!][!/*
        */!][!VAR "CanTxId" = "CanIfTxPduCanId"!][!/*
        */!][!IF "(CanIfTxPduCanIdType = 'EXTENDED_CAN') or (CanIfTxPduCanIdType = 'EXTENDED_FD_CAN')"!][!/*
          */!][!VAR "CanTxId" = "bit:bitset(num:i(CanIfTxPduCanId),31)"!][!/*
        */!][!ENDIF!][!/*
        */!][!IF "(CanIfTxPduCanIdType = 'STANDARD_FD_CAN') or (CanIfTxPduCanIdType = 'EXTENDED_FD_CAN')"!][!/*
          */!][!VAR "CanTxId" = "bit:bitset(num:i($CanTxId),30)"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ELSEIF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionFrIfCfgRef)"!][!/*
      */!][!VAR "SequenceCorrection"="XcpConnectionInterfaceType/XcpSequenceCorrectionEnabled"!][!/*
      */!][!VAR "FlxHeaderAlignment"="XcpConnectionInterfaceType/XcpFlxHeaderAlignment"!][!/*
      */!][!VAR "PackMultiMsgInOneFrame"="XcpConnectionInterfaceType/XcpPackMultiMsgInOneFlexRayFrame"!][!/*
      */!][!IF "XcpConnectionInterfaceType/XcpPackMultiMsgInOneFlexRayFrame = 'true' and ($FlxAssignEnabled = 'false') "!][!/*
        */!][!VAR "MaxMsgLengthInit"="XcpConnectionInterfaceType/XcpMaxFlexMsgLengthInit"!][!/*
      */!][!ENDIF!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionFrIfCfgRef)/FrIfPdu/*[(FrIfPduDirection='FrIfTxPdu') and node:refvalid(FrIfPduDirection/FrIfTxPduRef) and (node:ref(FrIfPduDirection/FrIfTxPduRef)/@name = $TxPduName) and ((text:toupper(FrIfPduDirection/FrIfUserTxUL) = 'XCP') or (text:toupper(FrIfPduDirection/FrIfUserTxUL) = 'CDD'))]"!][!/*
        */!][!VAR "ComIf" = "'FrIf'"!][!/*
        */!][!VAR "ComIfPduDestId" = "FrIfPduDirection/FrIfTxPduId"!][!/*
        */!][!IF "FrIfPduDirection/FrIfImmediate != 'true'"!][!/*
          */!][!VAR "TxFlags"="concat($TxFlags,'XCP_SUPPORT_FOR_TRIGGER_TRANSMIT',' ')"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ELSEIF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)"!][!/*
      */!][!VAR "PackMultiMsgInOneFrame"="XcpConnectionInterfaceType/XcpPackMultiMsgInOneEthernetFrame"!][!/*
      */!][!IF "XcpConnectionInterfaceType/XcpPackMultiMsgInOneEthernetFrame = 'true'"!][!/*
        */!][!VAR "MaxMsgLengthInit"="XcpConnectionInterfaceType/XcpMaxEthernetMsgLengthInit"!][!/*
      */!][!ENDIF!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxPduName)]"!][!/*
        */!][!VAR "ComIf" = "'SoAd'"!][!/*
        */!][!VAR "ComIfPduDestId" = "SoAdTxPduId"!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ENDIF!][!/*
    */!][!IF "XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!][!/*
      */!][!VAR "ComIf" = "'Cdd'"!][!/*
      */!][!VAR "ComIfPduDestId" = "node:ref($TxPduConnectionInfoPath)/XcpCddLowerLayerTxPduId"!][!/*
      */!][!IF "$TxPduCddTriggerTransmit = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_SUPPORT_FOR_TRIGGER_TRANSMIT',' ')"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "XcpMaxDataLength"="num:max(text:split(concat(XcpMaxCto, ' ', XcpMaxDto, ' ', XcpMaxCtoPgm)))"!][!/*
****************************************************************
* Get Pdu buffer size
****************************************************************
    */!][!CALL "GetPackagePduSize","XcpMaxDataLength"="$XcpMaxDataLength","PackMultiMsgInOneFrame"="$PackMultiMsgInOneFrame","MaxMsgLengthInit"="$MaxMsgLengthInit","XcpConnectionType"="XcpConnectionInterfaceType","FlxHeaderAlignment"="$FlxHeaderAlignment","SequenceCorrection"="$SequenceCorrection"!][!/*
****************************************************************
* Get the PDU supported package types(choises: CTO, EvCTO and DaqDTO)
****************************************************************
    */!][!SELECT "XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]"!][!/*
      */!][!IF "XcpPduSupportTxFromTxConfirmation = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_SUPPORT_TX_FROM_CONFIRMATION',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "XcpPduSupportForResErrCtoType = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_SUPPORT_FOR_RESERR_CTO_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "XcpPduSupportForEvServCtoType = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_SUPPORT_FOR_EVSERV_CTO_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "XcpPduSupportForDaqDtoType = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_SUPPORT_FOR_DAQ_DTO_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "node:exists(XcpDefaultStateDynamicTxPDU) = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_DYNAMIC_TX_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "node:exists(XcpDefaultStateDynamicTxPDU) = 'true' and XcpDefaultStateDynamicTxPDU = 'true'"!][!/*
        */!][!VAR "TxFlags"="concat($TxFlags,'XCP_ENABLE_STATE_DYNAMIC_TX_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDMACRO!][!/*

*************************************************************************
* MACRO to get the Rx Pdu info.
* Input parameters:
** "ConnectionPath" the XPath of the selected connection node(XcpConnectionCfg)
** "RxPduId" Xcp Rx Pdu internal ID
* Output parameters:
** "RxFlags" return the type of packages supported by the selected Rx Pdu(CmdCTO, StimDTO)
** "CanRxId" returns:
**           * the CAN Id, if available,
**           * empty string, otherwise.
** "XcpPackagePduSize" The Xcp Pdu size
** "OpenSoAdCon" Return if the Xcp will automatically open the socket connections
** "FlxAssign" Return if the connection supports FLX_ASSIGN command
*************************************************************************
*/!][!MACRO "GetRxPduInfo","ConnectionPath","RxPduId","RxPduName"!][!/*
  */!][!VAR "RxFlags" = "''"!][!/*
  */!][!VAR "CanRxId"="''"!][!/*
  */!][!VAR "PackMultiMsgInOneFrame"="'false'"!][!/*
  */!][!VAR "OpenSoAdCon"="'false'"!][!/*
  */!][!VAR "FlxAssign"="'false'"!][!/*
  */!][!VAR "MaxDlcRequired"="'false'"!][!/*
  */!][!SELECT "node:ref($ConnectionPath)"!][!/*
    */!][!VAR "PduMapIndex"="0"!][!/*
    */!][!VAR "FlxHeaderAlignment"="''"!][!/*
    */!][!VAR "SequenceCorrection"="'false'"!][!/*
    */!][!VAR "MaxMsgLengthInit"="0"!][!/*
    */!][!LOOP "node:order(XcpRxPduConnectionInfo/*, 'node:ref(XcpMapRxPdu2Connection)/XcpRxPduId')"!][!/*
      */!][!IF "node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId"!][!/*
        */!][!BREAK!][!/* we found the position
      */!][!ENDIF!][!/*
      */!][!VAR "PduMapIndex"="$PduMapIndex + 1"!][!/*
    */!][!ENDLOOP!][!/*
    */!][!IF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionCanIfCfgRef)"!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionCanIfCfgRef)/CanIfRxPduCfg/*[node:refvalid(CanIfRxPduRef) and (node:ref(CanIfRxPduRef)/@name = $RxPduName) and ((text:toupper(CanIfRxPduUserRxIndicationUL) = 'XCP') or (text:toupper(CanIfRxPduUserRxIndicationUL) = 'CDD'))]"!][!/*
        */!][!IF "CanIfRxPduCanIdType = 'EXTENDED_CAN'"!][!/*
          */!][!VAR "CanRxId" = "bit:bitset(num:i(CanIfRxPduCanId),31)"!][!/*
        */!][!ELSE!][!/*
          */!][!VAR "CanRxId" = "CanIfRxPduCanId"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ELSEIF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionCanFdCanIfCfgRef)"!][!/*
      */!][!VAR "MaxDlcRequired"="XcpConnectionInterfaceType/XcpCanFdMaxDlcRequired"!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionCanFdCanIfCfgRef)/CanIfRxPduCfg/*[node:refvalid(CanIfRxPduRef) and (node:ref(CanIfRxPduRef)/@name = $RxPduName) and ((text:toupper(CanIfRxPduUserRxIndicationUL) = 'XCP') or (text:toupper(CanIfRxPduUserRxIndicationUL) = 'CDD'))]"!][!/*
        */!][!VAR "CanRxId" = "CanIfRxPduCanId"!][!/*
        */!][!IF "(CanIfRxPduCanIdType = 'EXTENDED_CAN') or (CanIfRxPduCanIdType = 'EXTENDED_FD_CAN') or (CanIfRxPduCanIdType = 'EXTENDED_NO_FD_CAN')"!][!/*
          */!][!VAR "CanRxId" = "bit:bitset(num:i(CanIfRxPduCanId),31)"!][!/*
        */!][!ENDIF!][!/*
        */!][!IF "(CanIfRxPduCanIdType = 'STANDARD_FD_CAN') or (CanIfRxPduCanIdType = 'EXTENDED_FD_CAN')"!][!/*
          */!][!VAR "CanRxId" = "bit:bitset(num:i($CanRxId),30)"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ELSEIF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionFrIfCfgRef)"!][!/*
      */!][!VAR "FlxHeaderAlignment"="XcpConnectionInterfaceType/XcpFlxHeaderAlignment"!][!/*
      */!][!VAR "SequenceCorrection"="XcpConnectionInterfaceType/XcpSequenceCorrectionEnabled"!][!/*
      */!][!VAR "PackMultiMsgInOneFrame"="XcpConnectionInterfaceType/XcpPackMultiMsgInOneFlexRayFrame"!][!/*
      */!][!IF "XcpConnectionInterfaceType/XcpPackMultiMsgInOneFlexRayFrame = 'true' and ($FlxAssignEnabled = 'false')"!][!/*
        */!][!VAR "MaxMsgLengthInit"="XcpConnectionInterfaceType/XcpMaxFlexMsgLengthInit"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "($FlxAssignEnabled = 'true')"!][!/*
        */!][!VAR "FlxAssign"="'true'"!][!/*
      */!][!ENDIF!][!/*
    */!][!ELSEIF "node:refvalid(XcpConnectionInterfaceType/XcpConnectionSoAdConfigRef)"!][!/*
      */!][!VAR "PackMultiMsgInOneFrame"="XcpConnectionInterfaceType/XcpPackMultiMsgInOneEthernetFrame"!][!/*
      */!][!VAR "OpenSoAdCon"="XcpConnectionInterfaceType/XcpAutoOpenSoCon"!][!/*
      */!][!IF "XcpConnectionInterfaceType/XcpPackMultiMsgInOneEthernetFrame = 'true'"!][!/*
        */!][!VAR "MaxMsgLengthInit"="XcpConnectionInterfaceType/XcpMaxEthernetMsgLengthInit"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "XcpMaxDataLength"="num:max(text:split(concat(XcpMaxCto, ' ', XcpMaxDto, ' ', XcpMaxCtoPgm)))"!][!/*
****************************************************************
* Get Pdu buffer size
****************************************************************
    */!][!CALL "GetPackagePduSize","XcpMaxDataLength"="$XcpMaxDataLength","PackMultiMsgInOneFrame"="$PackMultiMsgInOneFrame","MaxMsgLengthInit"="$MaxMsgLengthInit","XcpConnectionType"="XcpConnectionInterfaceType","FlxHeaderAlignment"="$FlxHeaderAlignment","SequenceCorrection"="$SequenceCorrection"!][!/*
****************************************************************
* Get the PDU supported package types(choises: CMD, and StimDTO)
****************************************************************
    */!][!SELECT "XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]"!][!/*
      */!][!IF "XcpPduSupportRxFromRxIndication = 'true'"!][!/*
        */!][!VAR "RxFlags"="concat($RxFlags,'XCP_SUPPORT_RX_FROM_INDICATION',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "XcpPduSupportForCmdCtoType = 'true'"!][!/*
        */!][!VAR "RxFlags"="concat($RxFlags,'XCP_SUPPORT_FOR_CMD_CTO_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "XcpPduSupportForStimDtoType = 'true'"!][!/*
        */!][!VAR "RxFlags"="concat($RxFlags,'XCP_SUPPORT_FOR_STIM_DTO_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "node:exists(XcpDefaultStateDynamicRxPDU) = 'true'"!][!/*
        */!][!VAR "RxFlags"="concat($RxFlags,'XCP_DYNAMIC_RX_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "node:exists(XcpDefaultStateDynamicRxPDU) = 'true' and XcpDefaultStateDynamicRxPDU = 'true'"!][!/*
        */!][!VAR "RxFlags"="concat($RxFlags,'XCP_ENABLE_STATE_DYNAMIC_RX_PDU',' ')"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDMACRO!][!/*


*** Variable to check if FLX_ASSIGN is enabled ***
*/!][!VAR "FlxAssignEnabled"="(XcpGeneral/XcpOnFlexRayEnabled = 'true') and (XcpGeneral/XcpSupportForFlxTPCommands = 'true')"!][!/*



*** Variable to calculate the maximum Tx PDU Buffer size ***
*/!][!VAR "LocalTxPduBufferSize"="0"!][!/*
*** Variable to calculate the maximum Rx PDU Buffer size ***
*/!][!VAR "LocalRxPduBufferSize"="0"!][!/*


*** Variable to calculate the maximum between XcpMaxCto, XcpMaxCtoPgm and XcpMaxDto ***
*/!][!VAR "TxPduBufferSize"="0"!][!/*
*** Variable to calculate the maximum between XcpMaxCto, XcpMaxCtoPgm and XcpMaxDto ***
*/!][!VAR "MaxNoOfTxPdu"="0"!][!/*
*** Variable to calculate the maximum between XcpMaxCto, XcpMaxCtoPgm and XcpMaxDto ***
*/!][!VAR "RxPduBufferSize"="0"!][!/*
*** Variable to calculate the maximum between XcpMaxCto, XcpMaxCtoPgm and XcpMaxDto ***
*/!][!VAR "MaxNoOfRxPdu"="0"!][!/*
*/!][!LOOP "XcpConfig/*/XcpConnectionCfg/*"!][!/*
  */!][!CALL "GetRxPduInfo","ConnectionPath"="node:path(.)","RxPduId"="node:ref(XcpRxPduConnectionInfo/*[1]/XcpMapRxPdu2Connection)/XcpRxPduId","RxPduName"="node:ref(node:ref(XcpRxPduConnectionInfo/*[1]/XcpMapRxPdu2Connection)/XcpRxPduRef)/@name"!][!/*
  */!][!VAR "LocalMaxNoOfTxPdu"="count(XcpTxPduConnectionInfo/*)"!][!/*
  */!][!VAR "LocalMaxNoOfRxPdu"="count(XcpRxPduConnectionInfo/*)"!][!/*

  */!][!IF "(./XcpConnectionInterfaceType != 'XcpConnectionOverFlexRay') or ($FlxAssignEnabled = 'false')"!][!/*
    */!][!VAR "LocalTxPduBufferSize"="num:f(num:mul($LocalMaxNoOfTxPdu,$XcpPackagePduSize))"!][!/*
    */!][!VAR "LocalRxPduBufferSize"="num:f(num:mul($LocalMaxNoOfRxPdu,$XcpPackagePduSize))"!][!/*
  */!][!ELSE!][!/*

    */!][!LOOP "XcpConnectionInterfaceType/XcpFlexrayBufferCfg/*"!][!/*
      */!][!IF "(node:exists(FLX_BUFCfg/XcpFlxTxConnectionInfoRef) = 'true')"!][!/*
        */!][!IF "(node:exists(FLX_BUFCfg/XcpFlxBufMaxLenInitValue) = 'true')"!][!/*
           */!][!VAR "LocalTxPduBufferSize"="$LocalTxPduBufferSize + num:f(FLX_BUFCfg/XcpFlxBufMaxLenInitValue)"!][!/*
        */!][!ELSE!][!/*
          */!][!VAR "LocalTxPduBufferSize"="$LocalTxPduBufferSize + num:f(254)"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
      */!][!IF "(node:exists(FLX_BUFCfg/XcpFlxRxConnectionInfoRef) = 'true')"!][!/*
        */!][!IF "(node:exists(FLX_BUFCfg/XcpFlxBufMaxLenInitValue) = 'true')"!][!/*
           */!][!VAR "LocalRxPduBufferSize"="$LocalRxPduBufferSize + num:f(FLX_BUFCfg/XcpFlxBufMaxLenInitValue)"!][!/*
        */!][!ELSE!][!/*
          */!][!VAR "LocalRxPduBufferSize"="$LocalRxPduBufferSize + num:f(254)"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*

  */!][!ENDIF!][!/*


  */!][!IF "$LocalTxPduBufferSize > $TxPduBufferSize"!][!/*
    */!][!VAR "TxPduBufferSize"="$LocalTxPduBufferSize"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "$LocalRxPduBufferSize > $RxPduBufferSize"!][!/*
    */!][!VAR "RxPduBufferSize"="$LocalRxPduBufferSize"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "$LocalMaxNoOfTxPdu > $MaxNoOfTxPdu"!][!/*
    */!][!VAR "MaxNoOfTxPdu"="$LocalMaxNoOfTxPdu"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "$LocalMaxNoOfRxPdu > $MaxNoOfRxPdu"!][!/*
    */!][!VAR "MaxNoOfRxPdu"="$LocalMaxNoOfRxPdu"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*** Variable to calculate the maximum between PDU buffer size ***
*/!][!VAR "MaxLPduBufferSize"="node:when($TxPduBufferSize > $RxPduBufferSize, $TxPduBufferSize, $RxPduBufferSize)"!][!/*

*** Variable to calculate the number of Xcp connections over the Ethernet  ***
*/!][!VAR "NoOfXcpFlexRayCorrCon" = "(count(XcpConfig/*[1]/XcpConnectionCfg/*[(node:exists(XcpConnectionInterfaceType/XcpSequenceCorrectionEnabled) and (XcpConnectionInterfaceType/XcpSequenceCorrectionEnabled = 'true'))]))"!][!/*

*** Variable to calculate the number of Xcp connections over the Ethernet  ***
*/!][!VAR "NoOfXcpEthCon"="count(XcpConfig/*/XcpConnectionCfg/*[XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'])"!][!/*

*************************************************************************
* MACRO to print the property flags.
* "PrintPropertyFlags" String containing the property flag name
* "CommentString" String containing the comment information
*************************************************************************
*/!][!MACRO "PrintPropertyFlags","PropertyList","CommentString","Indentation"!][!/*
*/!][!IF "$PropertyList = ''"!][!/*
  */!][!INDENT "$Indentation"!][!/*
*/!]0U, /* [!"$CommentString"!] */
      [!ENDINDENT!][!/*
*/!][!ELSEIF "count(text:split($PropertyList)) = 1"!][!/*
  */!][!INDENT "$Indentation"!][!/*
*/!][!"text:split($PropertyList)[1]"!], /* [!"$CommentString"!] */
      [!ENDINDENT!][!/*
*/!][!ELSE!][!/*
  */!][!VAR "CurrentPos"="0"!][!/*
  */!][!LOOP "text:split($PropertyList)"!][!/*
    */!][!IF "$CurrentPos = 0"!][!/*
      */!][!INDENT "$Indentation"!][!/*
*/!]([!"."!] |
          [!ENDINDENT!][!/*
    */!][!ELSEIF "$CurrentPos = count(text:split($PropertyList)) - 1"!][!/*
      */!][!INDENT "$Indentation + 1"!][!/*
*/!][!"."!]), /* [!"$CommentString"!] */
          [!ENDINDENT!][!/*
    */!][!ELSE!][!/*
      */!][!INDENT "$Indentation + 1"!][!/*
*/!] [!"."!] |
          [!ENDINDENT!][!/*
    */!][!ENDIF!][!/*
  */!][!VAR "CurrentPos"="$CurrentPos + 1"!][!/*
  */!][!ENDLOOP!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

**************************************************************************
*** Variable to calculate the maximum number of CanIf Rx Pdus ***
*/!][!VAR "NoOfCanIfRxPdus"="0"!][!/*
*** Variable to calculate the maximum number of FrIf Rx Pdus ***
*/!][!VAR "NoOfFrIfRxPdus"="0"!][!/*
*** Variable to calculate the maximum number of SoAd Rx Pdus ***
*/!][!VAR "NoOfSoAdRxPdus"="0"!][!/*
*** Variable to calculate the maximum number of Cdd Rx Pdus ***
*/!][!VAR "NoOfCddRxPdus"="0"!][!/*
*** Get the maximum number of Lower Layer Rx Pdu Ids per underlay communication layer type ***
*/!][!LOOP "node:order(XcpConfig/*[1]/XcpPdu/*[@name='XcpRxPdu'], 'XcpRxPduId')"!][!/*
  */!][!VAR "RxPduId" = "XcpRxPduId"!][!/*
  */!][!SELECT "../../XcpConnectionCfg/*[count(XcpRxPduConnectionInfo/*[node:refvalid(XcpMapRxPdu2Connection) and (node:ref(XcpMapRxPdu2Connection)/XcpRxPduId = $RxPduId)]) > 0]"!][!/*
    */!][!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverCAN') or (XcpConnectionInterfaceType = 'XcpConnectionOverCANFD')"!][!/*
      */!][!VAR "NoOfCanIfRxPdus"="$NoOfCanIfRxPdus + 1"!][!/*
    */!][!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay'"!][!/*
      */!][!VAR "NoOfFrIfRxPdus"="$NoOfFrIfRxPdus + 1"!][!/*
    */!][!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!][!/*
      */!][!VAR "NoOfSoAdRxPdus"="$NoOfSoAdRxPdus + 1"!][!/*
    */!][!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!][!/*
      */!][!VAR "NoOfCddRxPdus"="$NoOfCddRxPdus + 1"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDLOOP!][!/*
**************************************************************************

**************************************************************************
*** Variable to calculate the maximum number of CanIf Tx Pdus ***
*/!][!VAR "NoOfCanIfTxPdus"="0"!][!/*
*** Variable to calculate the maximum number of FrIf Tx Pdus ***
*/!][!VAR "NoOfFrIfTxPdus"="0"!][!/*
*** Variable to calculate the maximum number of SoAd Tx Pdus ***
*/!][!VAR "NoOfSoAdTxPdus"="0"!][!/*
*** Variable to calculate the maximum number of Cdd Tx Pdus ***
*/!][!VAR "NoOfCddTxPdus"="0"!][!/*
*** Get the maximum number of Lower Layer Tx Pdu Ids per underlay communication layer type ***
*/!][!LOOP "node:order(XcpConfig/*[1]/XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!/*
  */!][!VAR "TxPduId" = "XcpTxPduId"!][!/*
  */!][!SELECT "../../XcpConnectionCfg/*[count(XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]) > 0]"!][!/*
    */!][!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverCAN') or (XcpConnectionInterfaceType = 'XcpConnectionOverCANFD')"!][!/*
      */!][!VAR "NoOfCanIfTxPdus"="$NoOfCanIfTxPdus + 1"!][!/*
    */!][!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay'"!][!/*
      */!][!VAR "NoOfFrIfTxPdus"="$NoOfFrIfTxPdus + 1"!][!/*
    */!][!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverEthernet'"!][!/*
      */!][!VAR "NoOfSoAdTxPdus"="$NoOfSoAdTxPdus + 1"!][!/*
    */!][!ELSEIF "XcpConnectionInterfaceType = 'XcpConnectionOverCDD'"!][!/*
      */!][!VAR "NoOfCddTxPdus"="$NoOfCddTxPdus + 1"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDLOOP!][!/*
**************************************************************************

**************************************************************************
*** Variable to determine if there is at least one FrIf Tx Pdu with decoupled access (in order to have the Trigger Transmit or not) ***
*/!][!VAR "TriggerTransmitIsEnabled"="'false'"!][!/*

*** Get the maximum number of Lower Layer Tx Pdu Ids per underlay communication layer type ***
*/!][!LOOP "node:order(XcpConfig/*[1]/XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!/*
  */!][!VAR "TxPduId" = "XcpTxPduId"!][!/*
  */!][!VAR "TxPduName"="node:ref(XcpTxPduRef)/@name"!][!/*
  */!][!SELECT "../../XcpConnectionCfg/*[count(XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]) > 0]"!][!/*
    */!][!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverFlexRay')"!][!/*
      */!][!SELECT "node:ref(XcpConnectionInterfaceType/XcpConnectionFrIfCfgRef)/FrIfPdu/*[(FrIfPduDirection='FrIfTxPdu') and node:refvalid(FrIfPduDirection/FrIfTxPduRef) and (node:ref(FrIfPduDirection/FrIfTxPduRef)/@name = $TxPduName) and ((text:toupper(FrIfPduDirection/FrIfUserTxUL) = 'XCP') or (text:toupper(FrIfPduDirection/FrIfUserTxUL) = 'CDD'))]"!][!/*
        */!][!IF "FrIfPduDirection/FrIfImmediate != 'true'"!][!/*
          */!][!VAR "TriggerTransmitIsEnabled"="'true'"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDSELECT!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDLOOP!][!/*
**************************************************************************

**************************************************************************
*** Variable to determine if there is at least one Cdd Tx Pdu with decoupled access (in order to have the Cdd Trigger Transmit or not) ***
*/!][!VAR "CddTriggerTransmitIsEnabled"="'false'"!][!/*

*** Get the maximum number of Lower Layer Tx Pdu Ids per underlay communication layer type ***
*/!][!LOOP "node:order(XcpConfig/*[1]/XcpPdu/*[@name='XcpTxPdu'], 'XcpTxPduId')"!][!/*
  */!][!VAR "TxPduId" = "XcpTxPduId"!][!/*
  */!][!VAR "TxPduName"="node:ref(XcpTxPduRef)/@name"!][!/*
  */!][!SELECT "../../XcpConnectionCfg/*[count(XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]) > 0]"!][!/*
    */!][!IF "(XcpConnectionInterfaceType = 'XcpConnectionOverCDD')"!][!/*
      */!][!LOOP "XcpTxPduConnectionInfo/*[node:refvalid(XcpMapTxPdu2Connection) and (node:ref(XcpMapTxPdu2Connection)/XcpTxPduId = $TxPduId)]"!][!/*
        */!][!IF "node:ref(XcpMapTxPdu2Connection)/XcpTxPduSupportForCddTriggerTransmit = 'true'"!][!/*
          */!][!VAR "CddTriggerTransmitIsEnabled"="'true'"!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDLOOP!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDSELECT!][!/*
*/!][!ENDLOOP!][!/*
**************************************************************************

**************************************************************************
*** Variable to determine the short-name, header file and transmit function of the configured CDD module ***
*/!][!VAR "CddShortNameList"="''"!][!/*
*/!][!VAR "CddHeaderFileList" = "''"!][!/*
*/!][!VAR "CddTemporaryHeaderFileList" = "''"!][!/*

*/!][!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*"!][!/*
  */!][!IF "(./XcpConnectionInterfaceType = 'XcpConnectionOverCDD')"!][!/*
    */!][!VAR "CddTemporaryHeaderFileList" = "concat($CddTemporaryHeaderFileList, node:value(./XcpConnectionInterfaceType/XcpCddInformation/XcpCddHeaderFile), ' ')"!][!/*
    */!][!IF "text:uniq(text:split($CddTemporaryHeaderFileList, ' '), node:value(./XcpConnectionInterfaceType/XcpCddInformation/XcpCddHeaderFile))"!][!/*
      */!][!VAR "CddHeaderFileList" = "concat($CddHeaderFileList, node:value(./XcpConnectionInterfaceType/XcpCddInformation/XcpCddHeaderFile), ' ')"!][!/*
      */!][!VAR "CddShortNameList" = "concat($CddShortNameList, node:value(./XcpConnectionInterfaceType/XcpCddInformation/XcpCddShortName), ' ')"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

**************************************************************************

**************************************************************************
*** Xcp MultiCore artifacts ***
*/!][!VAR "XcpBswDistributionEnabled" = "node:exists(XcpBswDistribution)"!][!/*
*/!][!VAR "XcpMemProxyOsAppRefName"="''"!][!/*
*/!][!VAR "XcpMemProxyOsAppRefNameList" = "''"!][!/*
*/!][!VAR "XcpConnectionOsAppRefName"="''"!][!/*
*/!][!VAR "XcpConnectionOsAppRefNameList" = "''"!][!/*
*/!][!VAR "XcpConnectionInstancesCount" = "0"!][!/*
*/!][!VAR "XcpMemProxyInstancesCount" = "0"!][!/*
*/!][!VAR "XcpCalibrationMemoryAccessAreas"="'false'"!][!/*
*/!][!VAR "XcpDaqStimMemoryAccessAreas"="'false'"!][!/*
*/!][!VAR "XcpMemoryAccessAreas"="'false'"!][!/*
*/!][!VAR "XcpMasterApplication"="''"!][!/*
*/!][!VAR "XcpRemoteDestinations"="''"!][!/*
*/!][!VAR "CorePartitionMap"="''"!][!/*

*/!][!IF "($XcpBswDistributionEnabled = 'true') and (XcpGeneral/XcpBlockWriteReadMemoryRAMMechanism='true')"!][!/*
*/!][!VAR "RAMCalloutProxyEnabled"="'true'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "RAMCalloutProxyEnabled"="'false'"!][!/*
*/!][!ENDIF!][!/*

*/!][!IF "$XcpBswDistributionEnabled"!][!/*

*/!][!LOOP "node:order(as:modconf('Xcp')[1]/XcpConfig/*[1]/XcpConnectionCfg/*,'XcpConnectionId')"!][!/*
*/!][!VAR "coreId"="'0'"!][!/*
*/!][!IF "node:exists(node:ref(XcpConnectionApplicationRef)/OsApplicationCoreAssignment)"!][!/*
*/!][!VAR "coreId"="node:ref(XcpConnectionApplicationRef)/OsApplicationCoreAssignment"!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "coreIdFound"="false()"!][!/*
*/!][!LOOP "text:split($CorePartitionMap,' ')"!][!/*
*/!][!IF "text:split(.,':')[1] = $coreId"!][!/*
*/!][!VAR "coreIdFound"="true()"!][!/*
*/!][!BREAK!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "$coreIdFound = 'false'"!][!/*
*/!][!VAR "CorePartitionMap"="concat($CorePartitionMap,' ',$coreId,':',node:name(node:ref(XcpConnectionApplicationRef)))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*/!][!VAR "XcpMasterApplication"="node:name(node:ref(XcpBswDistribution/XcpMasterApplicationRef))"!][!/*
*/!][!LOOP "XcpConfig/*[1]/XcpConnectionCfg/*"!][!/*
*/!][!IF "not(node:name(node:ref(XcpConnectionApplicationRef))=$XcpMasterApplication)"!][!/*
*/!][!VAR "XcpRemoteDestinations"="concat($XcpRemoteDestinations, ' ', node:name(node:ref(XcpConnectionApplicationRef)))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*/!][!LOOP "(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!][!/*
  */!][!IF "XcpMemoryAreaAcccessScope='ALL' or XcpMemoryAreaAcccessScope='CALIBRATION'"!][!/*
    */!][!VAR "XcpCalibrationMemoryAccessAreas"="'true'"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "XcpMemoryAreaAcccessScope='ALL' or XcpMemoryAreaAcccessScope='DAQ_STIM'"!][!/*
    */!][!VAR "XcpDaqStimMemoryAccessAreas"="'true'"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "count(XcpConfig/*[1]/XcpMemoryAccessArea/XcpMemoryAccessArea/*)>0"!][!/*
  */!][!VAR "XcpMemoryAccessAreas"="'true'"!][!/*
*/!][!ENDIF!][!/*


*/!][!SELECT "XcpConfig/*[1]"!][!/*
*/!][!LOOP "(XcpMemoryAccessArea/XcpMemoryAccessArea/*)"!][!/*
  */!][!VAR "XcpMemProxyOsAppRefName"="name(as:ref(./XcpMemoryAccessAreaApplicationRef))"!][!/*
  */!][!IF "not(contains($XcpMemProxyOsAppRefNameList,concat(' ',$XcpMemProxyOsAppRefName,' ')))"!][!/*
    */!][!VAR "XcpMemProxyInstancesCount" = "num:i(($XcpMemProxyInstancesCount + 1))"!][!/*
    */!][!VAR "XcpMemProxyOsAppRefNameList" = "concat($XcpMemProxyOsAppRefNameList,' ',$XcpMemProxyOsAppRefName,' ')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDSELECT!][!/*

*/!][!SELECT "XcpConfig/*[1]"!][!/*
*/!][!LOOP "(XcpConnectionCfg/*)"!][!/*
  */!][!VAR "XcpConnectionOsAppRefName"="name(as:ref(./XcpConnectionApplicationRef))"!][!/*
  */!][!IF "not(contains($XcpConnectionOsAppRefNameList,concat(' ',$XcpConnectionOsAppRefName,' ')))"!][!/*
    */!][!VAR "XcpConnectionInstancesCount" = "num:i(($XcpConnectionInstancesCount + 1))"!][!/*
    */!][!VAR "XcpConnectionOsAppRefNameList" = "concat($XcpConnectionOsAppRefNameList,' ',$XcpConnectionOsAppRefName,' ')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!ENDSELECT!][!/*

*/!][!VAR "XcpNumOsApps" = "0"!][!/*
*/!][!LOOP "(as:modconf('Os')[1]/OsApplication/*)"!][!/*
  */!][!VAR "XcpNumOsApps" = "num:i(($XcpNumOsApps) + 1)"!][!/*
*/!][!ENDLOOP!][!/*

*/!][!ENDIF!][!/*
**************************************************************************

*** End of file ***
*/!][!ENDIF!][!/*
*/!][!//
