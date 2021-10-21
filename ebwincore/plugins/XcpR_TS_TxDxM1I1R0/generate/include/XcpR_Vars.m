[!/**
 * \file
 *
 * \brief AUTOSAR XcpR
 *
 * This file contains the implementation of the AUTOSAR
 * module XcpR.
 *
 * \version 1.1.0
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!/*
*** multiple inclusion protection ***
*/!][!IF "not(var:defined('XCPR_VARS_M'))"!][!/*
*/!][!VAR "XCPR_VARS_M"="'true'"!][!/*

*/!][!VAR "NoOfDestFlexRayPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverFlexRay'])"!][!/*
*/!][!VAR "NoOfSrcFlexRayPdus"="count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverFlexRay'])"!][!/*
*/!][!VAR "NoOfDestCDDPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverCDD'])"!][!/*
*/!][!VAR "NoOfSrcCDDPdus"="count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverCDD'])"!][!/*
*/!][!VAR "NoOfDestCanFDPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverCANFD'])"!][!/*
*/!][!VAR "NoOfSrcCanFDPdus"="count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverCANFD'])"!][!/*
*/!][!VAR "NoOfDestCanPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[(XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverCAN')])"!][!/*
*/!][!VAR "NoOfSrcCanPdus"="count(XcpRGeneral/XcpRSourcePDUConfiguration/*[(XcpRSourceInterfaceType = 'XcpRSourceConnectionOverCAN')])"!][!/*
*/!][!VAR "NoOfDestEthernetPdus"="count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet'])"!][!/*
*/!][!VAR "NoOfSrcEthernetPdus"="count(XcpRGeneral/XcpRSourcePDUConfiguration/*[XcpRSourceInterfaceType = 'XcpRSourceConnectionOverEthernet'])"!][!/*

*** Ethernet: Variable to enable the automatically open socket connections ***
*/!][!VAR "XcpROpenSoCon" = "(count(XcpRGeneral/XcpRDestinationPDUConfiguration/*[(XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet') and (XcpRDestinationInterfaceType/XcpRDestinationOpenSoCon = 'true')]) > 0) or (count(XcpRGeneral/XcpRSourcePDUConfiguration/*[(XcpRSourceInterfaceType = 'XcpRSourceConnectionOverEthernet') and (XcpRSourceInterfaceType/XcpRSourceOpenSoCon = 'true')]) > 0)"!][!/*

*** Variable to enable the support for SoAd Socket Protocol Tcp ***
*/!][!VAR "XcpRSuportForSoAdSocketProtocolTcp" = "'false'"!][!/*
*/!][!VAR "NoOfXcpRTcpIpConnections" = "0"!][!/*
*/!][!LOOP "XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']"!][!/*
  */!][!VAR "TxDestPduName"="node:ref(./XcpRPdu/XcpRTxDestinationPduReference)/@name"!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!][!/*
    */!][!SELECT "node:ref(./XcpRDestinationInterfaceType/XcpRDestinationConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxDestPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketTcp')]"!][!/*
      */!][!VAR "XcpRSuportForSoAdSocketProtocolTcp" = "'true'"!][!/*
      */!][!VAR "NoOfXcpRTcpIpConnections" = "$NoOfXcpRTcpIpConnections + 1"!][!/*
    */!][!ENDSELECT!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*** Variable to enable the support for SoAd Socket Protocol Udp ***
*/!][!VAR "XcpRSuportForSoAdSocketProtocolUdp" = "'false'"!][!/*
*/!][!LOOP "XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType = 'XcpRDestinationConnectionOverEthernet']"!][!/*
  */!][!IF "./XcpRIsDestMasterConnection = 'true'"!][!/*
  */!][!VAR "TxDestPduName" = "node:ref(./XcpRPdu/XcpRTxDestinationPduReference)/@name"!][!/*
    */!][!IF "node:exists(as:modconf('SoAd')/SoAdConfig/*/SoAdPduRoute/*/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)"!][!/*
    */!][!SELECT "node:ref(./XcpRDestinationInterfaceType/XcpRDestinationConnectionSoAdConfigRef)/SoAdPduRoute/*[node:refvalid(SoAdTxPduRef) and (node:ref(SoAdTxPduRef)/@name = $TxDestPduName) and node:refvalid(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and (node:ref(SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../SoAdSocketProtocol = 'SoAdSocketUdp')]"!][!/*
      */!][!VAR "XcpRSuportForSoAdSocketProtocolUdp" = "'true'"!][!/*
      */!][!BREAK!][!/*
    */!][!ENDSELECT!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

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

*************************************************************************
* MACRO to get the pdu size.
* "FlxPackMultiMsgInOneFrame" used to decide if the packing of multiple PDUs in one frame is allowed/
* "FlxMaxMsgLengthInit" configured value for XcpMaxMsgLength[FlexRay]Init parameter
* "XcpRConnectionType" the type of the selected XcpR connection
* "FlxHeaderAlignment" configured value for XcpFlxHeaderAlignment parameter
*************************************************************************
*/!][!MACRO "GetPackagePduSize","FlxPackMultiMsgInOneFrame","EthPackMultiMsgInOneFrame","FlxMaxMsgLengthInit","EthMaxMsgLengthInit","XcpRConnectionType","FlxHeaderAlignment","SequenceCorrection", "XcpRMaxDataLength"!][!/*
  */!][!VAR "XcpRPackageHeaderSize" = "0"!][!/*
  */!][!IF "$XcpRConnectionType = 'XcpRSourceConnectionOverFlexRay' or $XcpRConnectionType = 'XcpRDestinationConnectionOverFlexRay'"!][!/*
    */!][!IF "$FlxPackMultiMsgInOneFrame = 'true'"!][!/*
          */!][!IF "$SequenceCorrection = 'true'"!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_8'"!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "3"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "4"!][!/*
              */!][!ENDIF!][!/*
          */!][!ELSE!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_32'"!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "4"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "2"!][!/*
              */!][!ENDIF!][!/*
          */!][!ENDIF!][!/*
    */!][!ELSE!][!/*
          */!][!IF "$SequenceCorrection = 'true'"!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_32'"!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "4"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "2"!][!/*
              */!][!ENDIF!][!/*
          */!][!ELSE!][!/*
              */!][!IF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_32'"!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "4"!][!/*
              */!][!ELSEIF "$FlxHeaderAlignment = 'PACKET_ALIGNMENT_16'"!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "2"!][!/*
              */!][!ELSE!][!/*
                 */!][!VAR "XcpRPackageHeaderSize" = "1"!][!/*
              */!][!ENDIF!][!/*
          */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "XcpRPackagePduSize" = "node:when(num:f(num:mod($XcpRPackageHeaderSize + $XcpRMaxDataLength, 2.0)) = 0, $XcpRPackageHeaderSize + $XcpRMaxDataLength, $XcpRPackageHeaderSize + $XcpRMaxDataLength + 1)"!][!/*
  */!][!ELSEIF "$XcpRConnectionType = 'XcpRSourceConnectionOverCANFD'"!][!/*
    */!][!IF "XcpRSourceInterfaceType/XcpRSourceCanFdMaxDlcRequired = 'true'"!][!/*
      */!][!VAR "XcpRPackagePduSize" = "XcpRSourceInterfaceType/XcpRSourceCanFdMaxDlc"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "XcpRPackagePduSize" = "$XcpRMaxDataLength"!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSEIF "$XcpRConnectionType = 'XcpRDestinationConnectionOverCANFD'"!][!/*
    */!][!IF "XcpRDestinationInterfaceType/XcpRDestinationCanFdMaxDlcRequired = 'true'"!][!/*
      */!][!VAR "XcpRPackagePduSize" = "XcpRDestinationInterfaceType/XcpRDestinationCanFdMaxDlc"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "XcpRPackagePduSize" = "$XcpRMaxDataLength"!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSEIF "$XcpRConnectionType = 'XcpRSourceConnectionOverEthernet' or $XcpRConnectionType = 'XcpRDestinationConnectionOverEthernet'"!][!/*
    */!][!VAR "XcpRPackageHeaderSize" = "4"!][!/*
    */!][!VAR "XcpRPackagePduSize" = "$XcpRPackageHeaderSize + $XcpRMaxDataLength"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "XcpRPackagePduSize" = "$XcpRMaxDataLength"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "$FlxPackMultiMsgInOneFrame = 'true'  and ($NoOfDestFlexRayPdus + $NoOfSrcFlexRayPdus > 0)"!][!/*
    */!][!VAR "XcpRPackagePduSize" = "$FlxMaxMsgLengthInit"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "$EthPackMultiMsgInOneFrame = 'true'  and ($NoOfDestEthernetPdus + $NoOfSrcEthernetPdus > 0)"!][!/*
    */!][!VAR "XcpRPackagePduSize" = "$EthMaxMsgLengthInit"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "GetFlxHeaderAlignment","FlxHeaderAlignment"!][!/*
  */!][!VAR "XcpRPackageAlignment" = "node:when($FlxHeaderAlignment = 'PACKET_ALIGNMENT_8', 1, node:when($FlxHeaderAlignment = 'PACKET_ALIGNMENT_16', 2, node:when($FlxHeaderAlignment = 'PACKET_ALIGNMENT_32', 4, 0)))"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "GetSourceConnectionType","InterfaceType"!][!/*
  */!][!IF "$InterfaceType = 'XcpRSourceConnectionOverCAN'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_CAN'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRSourceConnectionOverCANFD'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_CANFD'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRSourceConnectionOverFlexRay'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_FLEXRAY'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRSourceConnectionOverCDD'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_CDD'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRSourceConnectionOverEthernet'"!][!/*
     */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_ETHERNET'"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "GetDestinationConnectionType","InterfaceType"!][!/*
  */!][!IF "$InterfaceType = 'XcpRDestinationConnectionOverCAN'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_CAN'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRDestinationConnectionOverCANFD'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_CANFD'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRDestinationConnectionOverFlexRay'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_FLEXRAY'"!][!/*
  */!][!ELSEIF "$InterfaceType = 'XcpRDestinationConnectionOverCDD'"!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_CDD'"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "ConnectionType" = "'XCPR_CONNECTION_OVER_ETHERNET'"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*** CDD Upper Layer variables ***
*/!][!VAR "CddULShortName" = "'Xcp'"!][!/*
*/!][!VAR "CddULHeaderFile" = "'XcpOnCdd_Cbk.h'"!][!/*

*/!][!LOOP "(XcpRGeneral/XcpRDestinationPDUConfiguration/*)"!][!/*
  */!][!IF "node:exists(./XcpRDestUpperLayerInformation/XcpRUpperLayerModuleShortName)"!][!/*
    */!][!VAR "CddULShortName" = "XcpRDestUpperLayerInformation/XcpRUpperLayerModuleShortName"!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "node:exists(./XcpRDestUpperLayerInformation/XcpRUpperLayerHeaderFile)"!][!/*
    */!][!VAR "CddULHeaderFile" = "XcpRDestUpperLayerInformation/XcpRUpperLayerHeaderFile"!][!/*
  */!][!ENDIF!][!/*

  */!][!BREAK!][!/*
*/!][!ENDLOOP!][!/*

*** CDD Lower layer variables ***
*/!][!VAR "CddShortName" = "'Cdd'"!][!/*
*/!][!VAR "CddHeaderFileList" = "''"!][!/*

*/!][!LOOP "(XcpRGeneral/XcpRDestinationPDUConfiguration/*[XcpRDestinationInterfaceType='XcpRDestinationConnectionOverCDD' and (XcpRPdu = 'XcpRTxPdu')])"!][!/*
  */!][!VAR "CddShortName" = "XcpRDestinationInterfaceType/XcpRCddInformation/XcpRCddShortName"!][!/*
  */!][!IF "not(contains($CddHeaderFileList, concat(' ', XcpRDestinationInterfaceType/XcpRCddInformation/XcpRCddHeaderFile, ' ')))"!][!/*
    */!][!VAR "CddHeaderFileList" = "concat($CddHeaderFileList, XcpRDestinationInterfaceType/XcpRCddInformation/XcpRCddHeaderFile, ' ')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

*************************************************************************
* MACRO to get the callback functions for lower layer.
* "Connection" used to decide the type of connection for which we retrieve the function pointers
* "PDUType" type of the PDU Rx or Tx
*************************************************************************
*/!][!MACRO "GetCallbackFunction","Connection","PDUType", "NrOfTxSources"!][!/*
  */!][!IF "$Connection = 'XCPR_CONNECTION_OVER_CDD'"!][!/*
    */!][!IF "$PDUType = 'Tx'"!][!/*
        */!][!VAR "RxIndicationFunc" = "'NULL_PTR'"!][!/*
        */!][!IF "XcpRIsDestMasterConnection = 'true' and ($NrOfTxSources > 0)"!][!/*
            */!][!VAR "TxConfirmationFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRTxConfirmationFunctionName)"!][!/*
            */!][!IF "XcpRPdu/XcpRTxDestinationPduSupportForCddTriggerTransmit = 'true'"!][!/*
                */!][!VAR "TriggerTransmitFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRTriggerTransmitFunctionName)"!][!/*
            */!][!ELSE!][!/*
                */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
            */!][!ENDIF!][!/*
        */!][!ELSE!][!/*
            */!][!VAR "TxConfirmationFunc" = "'NULL_PTR'"!][!/*
            */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
        */!][!ENDIF!][!/*
        */!][!VAR "TransmitFunc" = "concat('&',XcpRDestinationInterfaceType/XcpRCddInformation/XcpRCddTransmitFunctionName)"!][!/*
    */!][!ELSE!][!/*
        */!][!VAR "RxIndicationFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRRxIndicationFunctionName)"!][!/*
        */!][!VAR "TxConfirmationFunc" = "'NULL_PTR'"!][!/*
        */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
        */!][!VAR "TransmitFunc" = "'NULL_PTR'"!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSEIF "($Connection = 'XCPR_CONNECTION_OVER_CANFD') or ($Connection = 'XCPR_CONNECTION_OVER_CAN')"!][!/*
        */!][!VAR "RxIndicationFunc" = "'NULL_PTR'"!][!/*
        */!][!IF "XcpRIsDestMasterConnection = 'true' and ($NrOfTxSources > 0)"!][!/*
            */!][!VAR "TxConfirmationFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRTxConfirmationFunctionName)"!][!/*
        */!][!ELSE!][!/*
            */!][!VAR "TxConfirmationFunc" = "'NULL_PTR'"!][!/*
        */!][!ENDIF!][!/*
        */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
        */!][!VAR "TransmitFunc" = "'&CanIf_Transmit'"!][!/*
  */!][!ELSEIF "$Connection = 'XCPR_CONNECTION_OVER_FLEXRAY'"!][!/*
        */!][!VAR "RxIndicationFunc" = "'NULL_PTR'"!][!/*
        */!][!IF "XcpRIsDestMasterConnection = 'true' and ($NrOfTxSources > 0)"!][!/*
            */!][!VAR "TxConfirmationFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRTxConfirmationFunctionName)"!][!/*
            */!][!IF "XcpRPdu/XcpRTxDestinationPduSupportForFrTriggerTransmit = 'true'"!][!/*
                */!][!VAR "TriggerTransmitFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRTriggerTransmitFunctionName)"!][!/*
            */!][!ELSE!][!/*
                */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
            */!][!ENDIF!][!/*
        */!][!ELSE!][!/*
            */!][!VAR "TxConfirmationFunc" = "'NULL_PTR'"!][!/*
            */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
        */!][!ENDIF!][!/*
        */!][!VAR "TransmitFunc" = "'&FrIf_Transmit'"!][!/*
  */!][!ELSE!][!/*
        */!][!VAR "RxIndicationFunc" = "'NULL_PTR'"!][!/*
        */!][!IF "XcpRIsDestMasterConnection = 'true' and ($NrOfTxSources > 0)"!][!/*
            */!][!VAR "TxConfirmationFunc" = "concat('&',XcpRDestUpperLayerInformation/XcpRTxConfirmationFunctionName)"!][!/*
        */!][!ELSE!][!/*
            */!][!VAR "TxConfirmationFunc" = "'NULL_PTR'"!][!/*
        */!][!ENDIF!][!/*
        */!][!VAR "TriggerTransmitFunc" = "'NULL_PTR'"!][!/*
        */!][!VAR "TransmitFunc" = "'&SoAd_IfTransmit'"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

*** End of file ***
*/!][!ENDIF!][!/*
*/!][!//