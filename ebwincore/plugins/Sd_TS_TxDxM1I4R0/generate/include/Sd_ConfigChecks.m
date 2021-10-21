[!/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]

[!// This file contains code generator checks

[!IF "not(var:defined('SD_CONFIGCHECKS'))"!]
[!VAR "SD_CONFIGCHECKS"="'true'"!]

[!SELECT "as:modconf('Sd')[1]/SdConfig/*[1]/SdInstance"!][!// Select first SdConfig

[!LOOP "*"!][!// Loop over SdInstances

  [!/* ------ Check Sd Control Path Configuration ------ */!]

  [!/* ------ Check Multicast Path ------ */!]
  [!IF "node:refvalid(as:modconf('SoAd')[1]//SoAdSocketRouteDest/*[SoAdRxPduRef = node:current()/SdInstanceMulticastRxPdu/SdRxPduRef]/../../SoAdRxSocketConnOrSocketConnBundleRef)"!]
    [!SELECT "as:ref(as:modconf('SoAd')[1]//SoAdSocketRouteDest/*[SoAdRxPduRef = node:current()/SdInstanceMulticastRxPdu/SdRxPduRef]/../../SoAdRxSocketConnOrSocketConnBundleRef)/../.."!]
      [!IF "node:refvalid(SoAdSocketLocalAddressRef)"!]
        [!IF "as:ref(SoAdSocketLocalAddressRef)/TcpIpAddressType != 'TCPIP_MULTICAST'"!]
          [!WARNING!]The SoAdSocketConnectionGroup used for the multicast Sd control path ([!"@name"!]) should be configured with a multicast TcpIp address ([!"as:ref(SoAdSocketLocalAddressRef)/@name"!])!"[!ENDWARNING!]
        [!ENDIF!][!// IF multicast TcpIpAddr
      [!ENDIF!][!// IF LocalAddressRef valid
    [!ENDSELECT!][!// SELECT Multicast SocketConnection
  [!ENDIF!][!// IF refvalid Multicast SocketConnection Ref

  [!/* ------ Check Unicast Path ------ */!]
  [!IF "node:refvalid(as:modconf('SoAd')[1]//SoAdPduRoute/*[SoAdTxPduRef = node:current()/SdInstanceTxPdu/SdTxPduRef]/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef) and node:refvalid(as:modconf('SoAd')[1]//SoAdSocketRouteDest/*[SoAdRxPduRef = node:current()/SdInstanceUnicastRxPdu/SdRxPduRef]/../../SoAdRxSocketConnOrSocketConnBundleRef)"!]
    [!IF "as:ref(as:modconf('SoAd')[1]//SoAdPduRoute/*[SoAdTxPduRef = node:current()/SdInstanceTxPdu/SdTxPduRef]/SoAdPduRouteDest/*/SoAdTxSocketConnOrSocketConnBundleRef)/../../@name != as:ref(as:modconf('SoAd')[1]//SoAdSocketRouteDest/*[SoAdRxPduRef = node:current()/SdInstanceUnicastRxPdu/SdRxPduRef]/../../SoAdRxSocketConnOrSocketConnBundleRef)/../../@name"!]
      [!WARNING "Unicast PDUs are not in same SoAdSocketConnectionGroup"!]
    [!ELSE!]
    [!SELECT "as:ref(as:modconf('SoAd')[1]//SoAdSocketRouteDest/*[SoAdRxPduRef = node:current()/SdInstanceUnicastRxPdu/SdRxPduRef]/../../SoAdRxSocketConnOrSocketConnBundleRef)/../.."!]
      [!IF "node:refvalid(SoAdSocketLocalAddressRef)"!]
        [!IF "as:ref(SoAdSocketLocalAddressRef)/TcpIpAddressType != 'TCPIP_UNICAST'"!]
          [!WARNING!]The SoAdSocketConnectionGroup used for the unicast Sd control path ([!"@name"!]) should be configured with a unicast TcpIp address ([!"as:ref(SoAdSocketLocalAddressRef)/@name"!])![!ENDWARNING!]
        [!ENDIF!][!// IF unicast TcpIpAddr
      [!ENDIF!][!// IF LocalAddressRef valid
    [!ENDSELECT!]
    [!ENDIF!]
  [!ENDIF!]
  
[!ENDLOOP!][!// LOOP SdInstances

[!ENDSELECT!][!// SELECT first SdConfig

[!SELECT "SdConfig/*[1]"!][!//

[!VAR "ConsumEventGroupMulticastRefCount"!][!"num:i(0)"!][!ENDVAR!][!//
[!VAR "EventgroupIndex"!][!"num:i(0)"!][!ENDVAR!][!//
[!LOOP "SdInstance/*/SdClientService/*/SdConsumedEventGroup/*"!][!//
[!IF "node:refvalid(SdConsumedEventGroupMulticastGroupRef/*)"!][!//
[!VAR "Match"!][!"num:i(0)"!][!ENDVAR!][!//
[!VAR "CurrSoConId"!][!"num:i(as:ref(SdConsumedEventGroupMulticastGroupRef/*[1])/SoAdSocketConnection/*[1]/SoAdSocketId)"!][!ENDVAR!][!//
[!VAR "CurrEventgroupIdx"!][!"num:i(0)"!][!ENDVAR!][!//
[!LOOP "../../../../../../SdInstance/*/SdClientService/*/SdConsumedEventGroup/*"!][!//
[!IF "$CurrEventgroupIdx < $EventgroupIndex"!][!//
[!IF "node:refvalid(SdConsumedEventGroupMulticastGroupRef/*[1])"!][!//
[!IF "$CurrSoConId = num:i(as:ref(SdConsumedEventGroupMulticastGroupRef/*[1])/SoAdSocketConnection/*[1]/SoAdSocketId)"!][!//
[!VAR "Match"!][!"num:i(1)"!][!ENDVAR!][!//
[!BREAK!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!VAR "CurrEventgroupIdx"!][!"num:i($CurrEventgroupIdx + 1)"!][!ENDVAR!][!//
[!ENDLOOP!][!//
[!IF "$Match = num:i(0)"!][!//
[!VAR "ConsumEventGroupMulticastRefCount"!][!"num:i($ConsumEventGroupMulticastRefCount + 1)"!][!ENDVAR!][!//
[!ENDIF!][!//
[!ENDIF!][!//
[!VAR "EventgroupIndex"!][!"num:i($EventgroupIndex + 1)"!][!ENDVAR!][!//
[!ENDLOOP!][!//
[!IF "num:i($ConsumEventGroupMulticastRefCount) > num:i(as:modconf('Sd')[1]/SdGeneral/SdMaxConsumedEventGroupMulticastReferences)"!]
[!ERROR "Total number of configured consumed event group multicast references should not exceed the value configured in SdGeneral/SdMaxConsumedEventGroupMulticastReferences"!]
[!ENDIF!]

[!ENDSELECT!][!//

[!ENDIF!][!// multiple inclusion protection

[!ENDNOCODE!]

