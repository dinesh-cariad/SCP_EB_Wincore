[!/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!SELECT "as:modconf('TcpIp')[1]"!][!/*

Calculate highest EthIf controller index by looping over all references
*/!][!VAR "MaxEthIfCtrlIdx" = "num:i(0)"!][!/*
*/!][!LOOP "TcpIpConfig/TcpIpLocalAddr/*"!][!/*
*/!][!VAR "TempCtrlIdx" = "num:i(node:value(node:ref(node:ref(./TcpIpCtrlRef)/TcpIpEthIfCtrlRef)/EthIfCtrlIdx))"!][!/*
*/!][!IF "$TempCtrlIdx > $MaxEthIfCtrlIdx"!][!/*
*/!][!VAR "MaxEthIfCtrlIdx" = "num:i($TempCtrlIdx)"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

Calculate number of tcpip controllers
*/!][!VAR "NumOfTcpIpCtrlIdx" = "num:i(count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpCtrl/*))"!][!/*

import GuardedDefine macro
*/!][!INCLUDE "CommonMacros.m"!][!/*

* Macro for converting a max time (datatype: float, unit:  seconds) to
* ticks of a given period (datatype: int, unit: ticks)
* Use calculation formula from [ecuc_sws_7001] and issue warnings according to [ecuc_sws_7002]
* parameter TimerSec: max time to be converted in ticks [sec]
* parameter PeriodSec: period to represent TimerSec in Ticks (usually a Mainfunction) [sec]
* ---------------------------------------------------------------------------------------------
*/!][!MACRO "TimerSecToTicks_Max", "TimerSec", "PeriodSec"!][!/*
*/!][!VAR "TimerUs"="num:i($TimerSec * 1000000)"!][!/* max time in [us]
*/!][!VAR "PeriodUs"="num:i($PeriodSec * 1000000)"!][!/* given period in [us]
*/!][!VAR "remainder" = "num:i($TimerUs mod $PeriodUs)"!][!/*
*/!][!IF "num:i($TimerUs div $PeriodUs) = 0"!][!/*
  */!][!ERROR "the calculated number of ticks is less than 1."!][!/*
*/!][!ENDIF!][!/*
*/!][!IF "$remainder != 0"!][!/*
  */!][!WARNING "the calculated number of ticks is not dividable without rest."!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i(floor($TimerUs div $PeriodUs))"!][!/*
*/!][!ENDMACRO!][!/*

* get Main function period [s] from xdm file
*/!][!VAR "MainFunctionPeriod_sec"="node:value(TcpIpGeneral/TcpIpMainFunctionPeriod)"!][!/*

*/!][!VAR "TimerInterval_1s"="1.0"!][!/*
*/!][!VAR "TimerInterval_100ms"="0.1"!][!/*

* get ARP timer interval [ticks] from xdm file
*/!][!VAR "TimerInterval_1s_ticks"!][!CALL "TimerSecToTicks_Max", "TimerSec"="$TimerInterval_1s", "PeriodSec"="$MainFunctionPeriod_sec"!][!ENDVAR!][!/*
*/!][!VAR "TimerInterval_100ms_ticks"!][!CALL "TimerSecToTicks_Max", "TimerSec"="$TimerInterval_100ms", "PeriodSec"="$MainFunctionPeriod_sec"!][!ENDVAR!][!/*

* Build a list of all local addresses sorted by TcpIpAddrId
*/!][!VAR "LocalAddrList" = "''"!][!/*
*/!][!LOOP "node:order(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpLocalAddr/*, 'TcpIpAddrId')"!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"./TcpIpDomainType"!];[!"./TcpIpAddressType"!];[!"num:i(as:ref(as:ref(TcpIpCtrlRef)/TcpIpEthIfCtrlRef)/EthIfCtrlIdx)"!];[!ENDVAR!][!/*
  */!][!IF "node:exists(./TcpIpStaticIpAddressConfig/TcpIpDefaultRouter)"!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"./TcpIpStaticIpAddressConfig/TcpIpDefaultRouter"!];[!ENDVAR!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]NONE;[!ENDVAR!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "node:exists(./TcpIpStaticIpAddressConfig/TcpIpNetmask)"!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"./TcpIpStaticIpAddressConfig/TcpIpNetmask"!];[!ENDVAR!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]NONE;[!ENDVAR!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "node:exists(./TcpIpStaticIpAddressConfig/TcpIpStaticIpAddress)"!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"./TcpIpStaticIpAddressConfig/TcpIpStaticIpAddress"!];[!ENDVAR!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]NONE;[!ENDVAR!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "(./TcpIpStaticIpAddressConfig/TcpIpStaticIpAddress = 'ANY')"!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]NONE;[!ENDVAR!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]solicitMlticastId[!"TcpIpAddrId"!];[!ENDVAR!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "node:refvalid(./TcpIpLocalAddrIPv6ExtHeaderFilterRef)"!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"num:i(count(as:ref(TcpIpLocalAddrIPv6ExtHeaderFilterRef)/TcpIpIpV6ConfigExtHeaderFilterEntry/*))"!];[!ENDVAR!][!/*
  */!][!ELSE!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"num:i(0)"!];[!ENDVAR!][!/*
  */!][!ENDIF!][!/*
  */!][!IF "node:refvalid(./TcpIpLocalAddrIPv6ExtHeaderFilterRef)"!][!/*
    */!][!VAR "FilterRef" = "''"!][!/*
    */!][!VAR "FilterRef" = "name(as:ref(TcpIpLocalAddrIPv6ExtHeaderFilterRef))"!][!/*
    */!][!VAR "index" = "0"!][!/*
    */!][!LOOP "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIpV6ConfigExtHeaderFilter/*)"!][!/*
      */!][!IF "$FilterRef = name(.)"!][!/*
        */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"num:i($index)"!];[!ENDVAR!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
      */!][!VAR "index" = "$index + num:i(count(./TcpIpIpV6ConfigExtHeaderFilterEntry/*))"!][!/*
    */!][!ENDLOOP!][!/*
  */!][!ELSE!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"num:i(0)"!];[!ENDVAR!][!/*
  */!][!ENDIF!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"count(./TcpIpAddrAssignment/*)"!];[!ENDVAR!][!/*
  */!][!LOOP "./TcpIpAddrAssignment/*"!][!/*
    */!][!IF "node:exists(./TcpIpAssignmentLifetime)"!][!/*
      */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"./TcpIpAssignmentLifetime"!];[!"./TcpIpAssignmentMethod"!];[!"./TcpIpAssignmentPriority"!];[!"./TcpIpAssignmentTrigger"!];[!"./TcpIpUseSimpleDhcpClient"!];[!ENDVAR!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]NONE;[!"./TcpIpAssignmentMethod"!];[!"./TcpIpAssignmentPriority"!];[!"./TcpIpAssignmentTrigger"!];[!"./TcpIpUseSimpleDhcpClient"!];[!ENDVAR!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!]|[!ENDVAR!][!/*
*/!][!ENDLOOP!][!/*

* Macro to add a static address to the local address list, e.g. DhcpV4 requires a limited broadcast address
*/!][!MACRO "LocalAddrList_addStaticMulticastEntry", "domain", "ctrlIdx", "ipAddr", "trigger"!][!/*
  */!][!VAR "LocalAddrList"!][!"$LocalAddrList"!][!"$domain"!];TCPIP_MULTICAST;[!"$ctrlIdx"!];NONE;NONE;[!"$ipAddr"!];NONE;0;0;1;NONE;TCPIP_STATIC;1;[!"$trigger"!];false;|[!ENDVAR!][!/*
*/!][!ENDMACRO!][!/*

* Index of a parameter in the local address list
*/!][!VAR "LocalAddrListIndexDomain" = "1"!][!/*
*/!][!VAR "LocalAddrListIndexType" = "2"!][!/*
*/!][!VAR "LocalAddrListIndexCtrlIdx" = "3"!][!/*
*/!][!VAR "LocalAddrListIndexDefaultRouter" = "4"!][!/*
*/!][!VAR "LocalAddrListIndexNetmask" = "5"!][!/*
*/!][!VAR "LocalAddrListIndexIpAddr" = "6"!][!/*
*/!][!VAR "LocalAddrListIndexMulticastRef" = "7"!][!/*
*/!][!VAR "LocalAddrListipV6ExtensionHeaderIdListNum" = "8"!][!/*
*/!][!VAR "LocalAddrListipV6ExtensionHeaderIdListOffset" = "9"!][!/*
*/!][!VAR "LocalAddrListIndexNumAssignMethods" = "10"!][!/*
*/!][!VAR "LocalAddrListFirstAssignOffset" = "11"!][!/*

* get the number of local addresses in the local address list
*/!][!MACRO "LocalAddrList_getNumLocalAddr"!][!/*
  */!][!"num:i(count(text:split($LocalAddrList,'|')))"!][!/*
*/!][!ENDMACRO!][!/*

* get the number of local addresses of domain in the local address list
*/!][!MACRO "LocalAddrList_getNumDomainLocalAddr", "domain"!][!/*
  */!][!VAR "NumDomainLocalAddr"="0"!][!/*
  */!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
  */!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
    */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
    */!][!IF "$domain = $localAddrDomain"!][!/*
      */!][!VAR "NumDomainLocalAddr"="$NumDomainLocalAddr + 1"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDFOR!][!/*
  */!][!"num:i($NumDomainLocalAddr)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getLocalAddr", "LocalAddrId"!][!/*
  */!][!"text:split($LocalAddrList,'|')[num:i($LocalAddrId) + 1]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getDomain", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexDomain)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getAddrType", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexType)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getCtrlIdx", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexCtrlIdx)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getDefaultRouter", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexDefaultRouter)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getNetmask", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexNetmask)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getIpAddr", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexIpAddr)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getMulticastRef", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexMulticastRef)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getNumAssignMethods", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListIndexNumAssignMethods)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getExtHdrListOffset", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListipV6ExtensionHeaderIdListOffset)]"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getExtHdrListNum", "LocalAddrId"!][!/*
  */!][!VAR "LocalAddr"!][!CALL "LocalAddrList_getLocalAddr", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!"text:split($LocalAddr,';')[num:i($LocalAddrListipV6ExtensionHeaderIdListNum)]"!][!/*
*/!][!ENDMACRO!][!/*

* Index of a parameter in method list
*/!][!VAR "LocalAddrListIndexLifetime" = "0"!][!/*
*/!][!VAR "LocalAddrListIndexMethod" = "1"!][!/*
*/!][!VAR "LocalAddrListIndexPrio" = "2"!][!/*
*/!][!VAR "LocalAddrListIndexTrigger" = "3"!][!/*
*/!][!VAR "LocalAddrListIndexUseSimpleDhcp" = "4"!][!/*
*/!][!VAR "NumOfMethodFields" = "5"!][!/*

*/!][!MACRO "LocalAddrList_getLifetimeOfMethod", "LocalAddrId", "method"!][!/*
  */!][!CALL "LocalAddrList_getFieldOfMethod", "LocalAddrId"="$LocalAddrId", "method"="$method", "fieldIdx"="$LocalAddrListIndexLifetime"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getMethodOfMethod", "LocalAddrId", "method"!][!/*
  */!][!CALL "LocalAddrList_getFieldOfMethod", "LocalAddrId"="$LocalAddrId", "method"="$method", "fieldIdx"="$LocalAddrListIndexMethod"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getPrioOfMethod", "LocalAddrId", "method"!][!/*
  */!][!CALL "LocalAddrList_getFieldOfMethod", "LocalAddrId"="$LocalAddrId", "method"="$method", "fieldIdx"="$LocalAddrListIndexPrio"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getTriggerOfMethod", "LocalAddrId", "method"!][!/*
  */!][!CALL "LocalAddrList_getFieldOfMethod", "LocalAddrId"="$LocalAddrId", "method"="$method", "fieldIdx"="$LocalAddrListIndexTrigger"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "LocalAddrList_getUseSimpleDhcpClientOfMethod", "LocalAddrId", "method"!][!/*
  */!][!CALL "LocalAddrList_getFieldOfMethod", "LocalAddrId"="$LocalAddrId", "method"="$method", "fieldIdx"="$LocalAddrListIndexUseSimpleDhcp"!][!/*
*/!][!ENDMACRO!][!/*

* loops over all methods for the local address and returns the value of the given method
*/!][!MACRO "LocalAddrList_getFieldOfMethod", "LocalAddrId", "method", "fieldIdx"!][!/*
  */!][!VAR "value"="'null'"!][!/*
  */!][!VAR "NumAssignMethods"!][!CALL "LocalAddrList_getNumAssignMethods", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!VAR "offset"="num:i($LocalAddrListFirstAssignOffset)"!][!/*
  */!][!FOR "x" = "1" TO "$NumAssignMethods"!][!/*
    */!][!VAR "assigMethod"="text:split($LocalAddr,';')[num:i($offset + $LocalAddrListIndexMethod)]"!][!/*
    */!][!IF "$assigMethod = $method"!][!/*
      */!][!VAR "value"="text:split($LocalAddr,';')[num:i($offset + $fieldIdx)]"!][!/*
      */!][!BREAK!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "offset"="num:i($offset + $NumOfMethodFields)"!][!/*
  */!][!ENDFOR!][!/*
  */!][!"$value"!][!/*
*/!][!ENDMACRO!][!/*

* Check for DHCPv4 assignment methods and add limited broadcast address to controller if none is explicitly configured
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "dhcpLocalAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$dhcpLocalAddrId"!][!ENDVAR!][!/*
  */!][!VAR "method"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$dhcpLocalAddrId", "method"="'TCPIP_DHCP'"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET') and ($method = 'TCPIP_DHCP')"!][!/*
    */!][!VAR "dhcpCtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$dhcpLocalAddrId"!][!ENDVAR!][!/*
    */!][!VAR "configured" = "0"!][!/*
    */!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
    */!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
      */!][!VAR "localAddrCtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
      */!][!VAR "addrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
      */!][!VAR "ipAddr"!][!CALL "LocalAddrList_getIpAddr", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
      */!][!IF "($dhcpCtrlIdx = $localAddrCtrlIdx) and ($addrType = 'TCPIP_MULTICAST') and ($ipAddr = '255.255.255.255')"!][!/*
        */!][!VAR "configured" = "1"!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
    */!][!IF "$configured = 0"!][!/*
      */!][!INFO!]Added limited broadcast address to controller [!"$dhcpCtrlIdx"!] which is required for [!/*
              */!]TCPIP_DHCP assignment (localAddressId [!"$dhcpLocalAddrId"!])[!ENDINFO!][!/*
      */!][!CALL "LocalAddrList_addStaticMulticastEntry", "domain"="'TCPIP_AF_INET'",
                                                          "ctrlIdx"="$dhcpCtrlIdx",
                                                          "ipAddr"="'255.255.255.255'",
                                                          "trigger"="'TCPIP_AUTOMATIC'"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

* Add limited broadcast address to controller if none is explicitly configured and there is an unicast IpV4 address
*/!][!IF "(TcpIpGeneral/TcpIpUdpAllowImplicitBroadcastReception = 'true')"!][!/*
  */!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
  */!][!FOR "ipV4LocalAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
    */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$ipV4LocalAddrId"!][!ENDVAR!][!/*
    */!][!VAR "localAddrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$ipV4LocalAddrId"!][!ENDVAR!][!/*
    */!][!IF "($localAddrDomain = 'TCPIP_AF_INET') and ($localAddrType = 'TCPIP_UNICAST')"!][!/*
      */!][!VAR "ipV4CtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$ipV4LocalAddrId"!][!ENDVAR!][!/*
      */!][!VAR "configured" = "0"!][!/*
      */!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
      */!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
        */!][!VAR "localAddrCtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
        */!][!VAR "addrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
        */!][!VAR "ipAddr"!][!CALL "LocalAddrList_getIpAddr", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
        */!][!IF "($ipV4CtrlIdx = $localAddrCtrlIdx) and ($addrType = 'TCPIP_MULTICAST') and ($ipAddr = '255.255.255.255')"!][!/*
          */!][!VAR "configured" = "1"!][!/*
          */!][!BREAK!][!/*
        */!][!ENDIF!][!/*
      */!][!ENDFOR!][!/*
      */!][!IF "$configured = 0"!][!/*
        */!][!INFO!]Added limited broadcast address to controller [!"$ipV4CtrlIdx"!] which is required for [!/*
                */!]ipv4 assignment (localAddressId [!"$ipV4LocalAddrId"!])[!ENDINFO!][!/*
        */!][!CALL "LocalAddrList_addStaticMulticastEntry", "domain"="'TCPIP_AF_INET'",
                                                            "ctrlIdx"="$ipV4CtrlIdx",
                                                            "ipAddr"="'255.255.255.255'",
                                                            "trigger"="'TCPIP_AUTOMATIC'"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDFOR!][!/*
*/!][!ENDIF!][!/*

*Check for IPv6 addresses on a controller, if a controller has a unicast IPv6 address
* but has no all nodes multicast explicitly configured add one
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "ipV6LocalAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$ipV6LocalAddrId"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET6')"!][!/*
    */!][!VAR "ipV6CtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$ipV6LocalAddrId"!][!ENDVAR!][!/*
    */!][!VAR "configured" = "0"!][!/*
    */!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
    */!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
      */!][!VAR "localAddrCtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
      */!][!VAR "addrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
      */!][!VAR "ipAddr"!][!CALL "LocalAddrList_getIpAddr", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
      */!][!IF "($ipV6CtrlIdx = $localAddrCtrlIdx) and ($addrType = 'TCPIP_MULTICAST') and ($ipAddr = 'ff02:0:0:0:0:0:0:1')"!][!/*
        */!][!VAR "configured" = "1"!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
    */!][!IF "$configured = 0"!][!/*
      */!][!INFO!]Added all nodes multicast address to controller [!"$ipV6CtrlIdx"!] which is required for [!/*
              */!]ipv6 assignment (localAddressId [!"$ipV6LocalAddrId"!])[!ENDINFO!][!/*
      */!][!CALL "LocalAddrList_addStaticMulticastEntry", "domain"="'TCPIP_AF_INET6'",
                                                          "ctrlIdx"="$ipV6CtrlIdx",
                                                          "ipAddr"="'ff02:0:0:0:0:0:0:1'",
                                                          "trigger"="'TCPIP_AUTOMATIC'"!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*


*Add IPv6 solicit multicast address for every IPv6 unicast address
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "ipV6LocalAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$ipV6LocalAddrId"!][!ENDVAR!][!/*
  */!][!VAR "addrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$ipV6LocalAddrId"!][!ENDVAR!][!/*
  */!][!VAR "multicastRef"!][!CALL "LocalAddrList_getMulticastRef", "LocalAddrId"="$ipV6LocalAddrId"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET6') and ($addrType = 'TCPIP_UNICAST') and ($multicastRef = concat('solicitMlticastId',$ipV6LocalAddrId))"!][!/*
    */!][!VAR "ipV6CtrlIdx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$ipV6LocalAddrId"!][!ENDVAR!][!/*
    */!][!INFO!]Added solicit node multicast address to controller [!"$ipV6CtrlIdx"!] which is required for [!/*
    */!]ipv6 assignment (localAddressId [!"$ipV6LocalAddrId"!])[!ENDINFO!][!/*
    */!][!CALL "LocalAddrList_addStaticMulticastEntry", "domain"="'TCPIP_AF_INET6'",
                                                        "ctrlIdx"="$ipV6CtrlIdx",
                                                        "ipAddr"="'NONE'",
                                                        "trigger"="'TCPIP_MANUAL'"!][!/*
    */!][!VAR "NumLocalAddr" = "num:i($NumLocalAddr + 1)"!][!/*
    */!][!VAR "LocalAddrList" = "text:replace($LocalAddrList, concat('solicitMlticastId',$ipV6LocalAddrId), num:i($NumLocalAddr - 1))"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "LocalAddrList" = "text:replace($LocalAddrList, concat('solicitMlticastId',$ipV6LocalAddrId), 'NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

* Check if maximal number of configured local addresses is big enough to hold all generated addresses
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumDomainLocalAddr", "domain"="'TCPIP_AF_INET'"!][!ENDVAR!][!/*
*/!][!VAR "LocalAddrIpv4EntriesMax"="TcpIpGeneral/TcpIpIpV4General/TcpIpLocalAddrIpv4EntriesMax"!][!/*
*/!][!IF "($NumLocalAddr > $LocalAddrIpv4EntriesMax)"!][!/*
  */!][!ERROR!]The number of IPv4 local addresses ([!"$NumLocalAddr"!]) must not exceed TcpIpLocalAddrIpv4EntriesMax [!/*
           */!]([!"$LocalAddrIpv4EntriesMax"!]).[!ENDERROR!][!/*
*/!][!ENDIF!][!/*

* Check if maximal number of configured local addresses is big enough to hold all generated addresses
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumDomainLocalAddr", "domain"="'TCPIP_AF_INET6'"!][!ENDVAR!][!/*
*/!][!VAR "LocalAddrIpv6EntriesMax"="TcpIpGeneral/TcpIpIpV6General/TcpIpLocalAddrIpv6EntriesMax"!][!/*
*/!][!IF "($NumLocalAddr > $LocalAddrIpv6EntriesMax)"!][!/*
  */!][!ERROR!]The number of IPv6 local addresses ([!"$NumLocalAddr"!]) must not exceed TcpIpLocalAddrIpv6EntriesMax [!/*
           */!]([!"$LocalAddrIpv6EntriesMax"!]).[!ENDERROR!][!/*
*/!][!ENDIF!][!/*

*/!][!MACRO "IpV4AddressToUINT32", "IpAddress", "endianCheck"!][!/*
*/!][!IF "($endianCheck = 'false') or (ecu:get('Cpu.Byteorder') = 'BE')"!][!/*
*/!][!VAR "factorList" = "'16777216.65536.256.1'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "factorList" = "'1.256.65536.16777216'"!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "ip" = "0"!][!/*
*/!][!FOR "x" = "1" TO "count(text:split($IpAddress,'.'))"!][!/*
*/!][!VAR "addrbyte" = "text:split($IpAddress,'.')[num:i($x)]"!][!/*
*/!][!VAR "factor" = "text:split($factorList,'.')[num:i($x)]"!][!/*
*/!][!VAR "ip" = "num:i($ip) + num:i($addrbyte)*num:i($factor)"!][!/*
*/!][!ENDFOR!][!/*
*/!][!"num:i($ip)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "IpV6AddressToUINT32", "IpAddress"!][!/*
*/!][!IF "ecu:get('Cpu.Byteorder') = 'LE'"!][!/*
*/!][!VAR "factorList" = "'1.256.65536.16777216'"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "factorList" = "'16777216.65536.256.1'"!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "ip" = "0"!][!/*
*/!][!VAR "y" = "1"!][!/*
*/!][!FOR "x" = "1" TO "count(text:split($IpAddress,':'))"!][!/*
*/!][!VAR "addrbyte_1" = "bit:shr(num:hextoint(text:split($IpAddress,':')[num:i($x)]), num:i(8))"!][!/*
*/!][!VAR "addrbyte_2" = "bit:and(num:hextoint(text:split($IpAddress,':')[num:i($x)]), num:i(255))"!][!/*
*/!][!VAR "factor_1" = "text:split($factorList,'.')[num:i($y)]"!][!/*
*/!][!VAR "factor_2" = "text:split($factorList,'.')[num:i($y + 1)]"!][!/*
*/!][!VAR "ip" = "num:i($ip) + num:i($addrbyte_1)*num:i($factor_1) + num:i($addrbyte_2)*num:i($factor_2)"!][!/*
*/!][!VAR "y"="$y + 2"!][!/*
*/!][!IF "$y > 4"!][!//

              <field>
                <int>[!"num:i($ip)"!]</int>
              </field>
[!VAR "ip" = "0"!][!/*
*/!][!VAR "y"="1"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "IpV6AddressToUINT32List", "IpAddress"!][!/*
*/!][!VAR "list" = "''"!][!/*
*/!][!VAR "factorList" = "'16777216.65536.256.1'"!][!/*
*/!][!VAR "ip" = "num:i(0)"!][!/*
*/!][!VAR "y" = "1"!][!/*
*/!][!FOR "x" = "1" TO "count(text:split($IpAddress,':'))"!][!/*
*/!][!VAR "addrbyte_1" = "bit:shr(num:hextoint(text:split($IpAddress,':')[num:i($x)]), num:i(8))"!][!/*
*/!][!VAR "addrbyte_2" = "bit:and(num:hextoint(text:split($IpAddress,':')[num:i($x)]), num:i(255))"!][!/*
*/!][!VAR "factor_1" = "text:split($factorList,'.')[num:i($y)]"!][!/*
*/!][!VAR "factor_2" = "text:split($factorList,'.')[num:i($y + 1)]"!][!/*
*/!][!VAR "ip" = "num:i($ip) + num:i($addrbyte_1)*num:i($factor_1) + num:i($addrbyte_2)*num:i($factor_2)"!][!/*
*/!][!VAR "y"="$y + 2"!][!/*
*/!][!IF "$y > 4"!][!/*
*/!][!VAR "list" = "concat($list, ' ', num:i($ip))"!][!/*
*/!][!VAR "ip" = "0"!][!/*
*/!][!VAR "y"="1"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDFOR!][!//
[!"$list"!][!//
[!ENDMACRO!][!/*

*/!][!MACRO "MACAddressToUINT32H", "MacAddress", "StartId"!][!/*
*/!][!VAR "factorList" = "'65536.256.1'"!][!/*
*/!][!VAR "ip" = "0"!][!/*
*/!][!FOR "x" = "$StartId" TO "($StartId + '2')"!][!/*
*/!][!VAR "addrbyte" = "num:hextoint(text:split($MacAddress,':')[num:i($x)])"!][!/*
*/!][!VAR "factor" = "text:split($factorList,'.')[num:i($x - $StartId + '1')]"!][!/*
*/!][!VAR "ip" = "num:i($ip) + num:i($addrbyte)*num:i($factor)"!][!/*
*/!][!ENDFOR!][!/*
*/!][!"num:i($ip)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "HammingWeight4", "mask"!][!/*
*/!][!"num:i(bit:and($mask, 1) + bit:and(bit:shr($mask, 1), 1) + bit:and(bit:shr($mask, 2), 1) + bit:and(bit:shr($mask, 3), 1))"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "AssignmentTypeToValue", "method", "type"!][!/*
*/!][!IF "($method = 'TCPIP_STATIC') and ($type = 'TCPIP_AF_INET')"!][!/*
  */!][!VAR "result"="0"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_LINKLOCAL') and ($type = 'TCPIP_AF_INET')"!][!/*
  */!][!VAR "result"="1"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_LINKLOCAL_DOIP') and ($type = 'TCPIP_AF_INET')"!][!/*
  */!][!VAR "result"="2"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_DHCP') and ($type = 'TCPIP_AF_INET')"!][!/*
  */!][!VAR "result"="3"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_STATIC') and ($type = 'TCPIP_AF_INET6')"!][!/*
  */!][!VAR "result"="4"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_LINKLOCAL') and ($type = 'TCPIP_AF_INET6')"!][!/*
  */!][!VAR "result"="5"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_DHCP') and ($type = 'TCPIP_AF_INET6')"!][!/*
  */!][!VAR "result"="6"!][!/*
*/!][!ELSEIF "($method = 'TCPIP_IPV6_ROUTER') and ($type = 'TCPIP_AF_INET6')"!][!/*
  */!][!VAR "result"="7"!][!/*
*/!][!ELSE!][!/*
  */!][!ERROR "Selected TcpIpAssignmentMethod is not supported"!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i($result)"!][!/*
*/!][!ENDMACRO!][!/*


*/!][!VAR "NumberOfAssignmentTypes" = "num:i(8)"!][!/*

*/!][!MACRO "DefendMechanismToValue", "defendMechanism"!][!/*
*/!][!IF "$defendMechanism = 'TCPIP_DISABLE'"!][!/*
  */!][!VAR "result"="0"!][!/*
*/!][!ELSEIF "$defendMechanism = 'TCPIP_ABANDON_ADDR'"!][!/*
  */!][!VAR "result"="1"!][!/*
*/!][!ELSEIF "$defendMechanism = 'TCPIP_DEFEND_ADDR'"!][!/*
  */!][!VAR "result"="2"!][!/*
*/!][!ELSEIF "$defendMechanism = 'TCPIP_DEFEND_INF_ADDR'"!][!/*
  */!][!VAR "result"="3"!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i($result)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!VAR "NumberOfConfiguredAssignmentTypes" = "num:i(4)"!][!/*

*/!][!VAR "IpV4StaticCount" = "num:i(0)"!][!/*
*/!][!VAR "IpV4StaticIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "IpV4StaticLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "localAddrMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_STATIC'"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET') and ($localAddrMethod = 'TCPIP_STATIC')"!][!/*
    */!][!VAR "IpV4StaticIdx2LocalAddrId" = "concat($IpV4StaticIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV4StaticLocalAddrId2Idx" = "concat($IpV4StaticLocalAddrId2Idx,' ',num:i($IpV4StaticCount))"!][!/*
    */!][!VAR "IpV4StaticCount" = "num:i($IpV4StaticCount + 1)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV4StaticLocalAddrId2Idx" = "concat($IpV4StaticLocalAddrId2Idx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*


*/!][!VAR "StaticAddrList" = "''"!][!/*
*/!][!VAR "StaticAddrCount" = "num:i(0)"!][!/*
*/!][!VAR "IpV6StaticCount" = "num:i(0)"!][!/*
*/!][!VAR "LocalAddrId2IpV6StaticIdx" = "''"!][!/*
*/!][!VAR "IpV6StaticIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "IpV6StaticLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "IpV6LinkLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "staticMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_STATIC'"!][!ENDVAR!][!/*
  */!][!VAR "linkLocalMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL'"!][!ENDVAR!][!/*
  */!][!VAR "ipAddr"!][!CALL "LocalAddrList_getIpAddr", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "netmask"!][!CALL "LocalAddrList_getNetmask", "LocalAddrId"="$LocalAddrId"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET6') and (($staticMethod = 'TCPIP_STATIC') or ($linkLocalMethod = 'TCPIP_LINKLOCAL'))"!][!/*
    */!][!IF "$linkLocalMethod = 'TCPIP_LINKLOCAL'"!][!/*
      */!][!VAR "IpV6LinkLocalAddrId2Idx" = "concat($IpV6LinkLocalAddrId2Idx,' ',num:i($IpV6StaticCount))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV6LinkLocalAddrId2Idx" = "concat($IpV6LinkLocalAddrId2Idx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
    */!][!IF "$staticMethod = 'TCPIP_STATIC'"!][!/*
      */!][!VAR "IpV6StaticLocalAddrId2Idx" = "concat($IpV6StaticLocalAddrId2Idx,' ',num:i($IpV6StaticCount))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV6StaticLocalAddrId2Idx" = "concat($IpV6StaticLocalAddrId2Idx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "IpV6StaticIdx2LocalAddrId" = "concat($IpV6StaticIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV6StaticCount" = "num:i($IpV6StaticCount + 1)"!][!/*
    */!][!IF "((($ipAddr != 'NONE') and ($ipAddr != 'ANY')) or ($netmask != 'NONE'))"!][!/*
      */!][!VAR "StaticAddrList"!][!"$StaticAddrList"!][!"num:i($StaticAddrCount)"!];[!ENDVAR!][!/*
      */!][!VAR "StaticAddrCount" = "num:i($StaticAddrCount + 1)"!][!/*
       */!][!VAR "LocalAddrId2IpV6StaticIdx" = "concat($LocalAddrId2IpV6StaticIdx,' ',num:i($localAddrId))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "StaticAddrList"!][!"$StaticAddrList"!][!"num:i(255)"!];[!ENDVAR!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV6StaticLocalAddrId2Idx" = "concat($IpV6StaticLocalAddrId2Idx,' NONE')"!][!/*
    */!][!VAR "IpV6LinkLocalAddrId2Idx" = "concat($IpV6LinkLocalAddrId2Idx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

*/!][!VAR "IpV4DhcpCount" = "num:i(0)"!][!/*
*/!][!VAR "IpV4DhcpIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "IpV4DhcpLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "method"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_DHCP'"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET') and ($method = 'TCPIP_DHCP')"!][!/*
    */!][!VAR "IpV4DhcpIdx2LocalAddrId" = "concat($IpV4DhcpIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV4DhcpLocalAddrId2Idx" = "concat($IpV4DhcpLocalAddrId2Idx,' ',num:i($IpV4DhcpCount))"!][!/*
    */!][!VAR "IpV4DhcpCount" = "num:i($IpV4DhcpCount + 1)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV4DhcpLocalAddrId2Idx" = "concat($IpV4DhcpLocalAddrId2Idx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

*/!][!VAR "IpV6DhcpCount" = "num:i(0)"!][!/*
*/!][!VAR "IpV6DhcpIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "IpV6DhcpLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "method"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_DHCP'"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET6') and ($method = 'TCPIP_DHCP')"!][!/*
    */!][!VAR "IpV6DhcpIdx2LocalAddrId" = "concat($IpV6DhcpIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV6DhcpLocalAddrId2Idx" = "concat($IpV6DhcpLocalAddrId2Idx,' ',num:i($IpV6DhcpCount))"!][!/*
    */!][!VAR "IpV6DhcpCount" = "num:i($IpV6DhcpCount + 1)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV6DhcpLocalAddrId2Idx" = "concat($IpV6DhcpLocalAddrId2Idx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

*/!][!VAR "IpV6NdpRpdCount" = "num:i(0)"!][!/*
*/!][!VAR "IpV6NdpRpdIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "IpV6NdpRpdLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "method"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_IPV6_ROUTER'"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET6') and ($method = 'TCPIP_IPV6_ROUTER')"!][!/*
    */!][!VAR "IpV6NdpRpdIdx2LocalAddrId" = "concat($IpV6NdpRpdIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV6NdpRpdLocalAddrId2Idx" = "concat($IpV6NdpRpdLocalAddrId2Idx,' ',num:i($IpV6NdpRpdCount))"!][!/*
    */!][!VAR "IpV6NdpRpdCount" = "num:i($IpV6NdpRpdCount + 1)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV6NdpRpdLocalAddrId2Idx" = "concat($IpV6NdpRpdLocalAddrId2Idx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*


*/!][!VAR "IpV6NdpDadEnable" = "'false'"!][!/*
*/!][!IF "(TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpNdpConfig/*[1]/TcpIpNdpSlaacConfig/TcpIpNdpSlaacDadNumberOfTransmissions > 0)"!][!/*
*/!][!VAR "IpV6NdpDadEnable" = "'true'"!][!/*
*/!][!ENDIF!][!/*

*/!][!VAR "IpV6NdpDadCount" = "num:i(0)"!][!/*
*/!][!IF "(TcpIpGeneral/TcpIpIpV6General/TcpIpIpV6Enabled = 'true') and ($IpV6NdpDadEnable = 'true')"!][!/*
*/!][!VAR "IpV6NdpDadIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "addrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET6') and ($addrType = 'TCPIP_UNICAST')"!][!/*
    */!][!VAR "IpV6NdpDadIdx2LocalAddrId" = "concat($IpV6NdpDadIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV6NdpDadCount" = "num:i($IpV6NdpDadCount + 1)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!ENDIF!][!/*

*/!][!VAR "IpNvMCount" = "num:i(0)"!][!/*
*/!][!IF "(node:exists(TcpIpConfig/TcpIpNvmBlock/TcpIpNvmBlockDescriptorRef) = 'true')"!][!/*
*/!][!VAR "IntNvMMemoryIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "iPMethodLifetime"!][!CALL "LocalAddrList_getLifetimeOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL'"!][!ENDVAR!][!/*
  */!][!IF "($iPMethodLifetime = 'TCPIP_STORE')"!][!/*
    */!][!VAR "IntNvMMemoryIdx2LocalAddrId" = "concat($IntNvMMemoryIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpNvMCount" = "num:i($IpNvMCount + 1)"!][!/*
  */!][!ENDIF!][!/*
  */!][!VAR "iPMethodLifetime"!][!CALL "LocalAddrList_getLifetimeOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL_DOIP'"!][!ENDVAR!][!/*
  */!][!IF "($iPMethodLifetime = 'TCPIP_STORE')"!][!/*
    */!][!VAR "IntNvMMemoryIdx2LocalAddrId" = "concat($IntNvMMemoryIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpNvMCount" = "num:i($IpNvMCount + 1)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!ENDIF!][!/*

* special handling for AUTO_IP and AUTO_IP_DOIP - as they will not be executed at the same time
* (invalid) they use the same method index for acessign RAM arrays (single implementation
*/!][!VAR "IpV4LinkLocalCount" = "num:i(0)"!][!/*
*/!][!VAR "IpV4LinkLocalIdx2LocalAddrId" = "''"!][!/*
*/!][!VAR "IpV4AutoIpLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "IpV4AutoIpDoIpLocalAddrId2Idx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "doipMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL_DOIP'"!][!ENDVAR!][!/*
  */!][!VAR "linkLocalMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL'"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET') and (($linkLocalMethod = 'TCPIP_LINKLOCAL') or ($doipMethod = 'TCPIP_LINKLOCAL_DOIP'))"!][!/*
    */!][!IF "$linkLocalMethod = 'TCPIP_LINKLOCAL'"!][!/*
      */!][!VAR "IpV4AutoIpLocalAddrId2Idx" = "concat($IpV4AutoIpLocalAddrId2Idx,' ',num:i($IpV4LinkLocalCount))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV4AutoIpLocalAddrId2Idx" = "concat($IpV4AutoIpLocalAddrId2Idx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
    */!][!IF "$doipMethod = 'TCPIP_LINKLOCAL_DOIP'"!][!/*
      */!][!VAR "IpV4AutoIpDoIpLocalAddrId2Idx" = "concat($IpV4AutoIpDoIpLocalAddrId2Idx,' ',num:i($IpV4LinkLocalCount))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV4AutoIpDoIpLocalAddrId2Idx" = "concat($IpV4AutoIpDoIpLocalAddrId2Idx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "IpV4LinkLocalIdx2LocalAddrId" = "concat($IpV4LinkLocalIdx2LocalAddrId,' ',num:i($localAddrId))"!][!/*
    */!][!VAR "IpV4LinkLocalCount" = "num:i($IpV4LinkLocalCount + 1)"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV4AutoIpLocalAddrId2Idx" = "concat($IpV4AutoIpLocalAddrId2Idx,' NONE')"!][!/*
    */!][!VAR "IpV4AutoIpDoIpLocalAddrId2Idx" = "concat($IpV4AutoIpDoIpLocalAddrId2Idx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

*/!][!VAR "TcpIpCtrlIdx2EthIfCtrlIdx" = "''"!][!/*
*/!][!VAR "EthIfCtrlIdx2TcpIpCtrlIdx" = "''"!][!/*
*/!][!FOR "x" = "0" TO "num:i($MaxEthIfCtrlIdx)"!][!/*
  */!][!VAR "referenced" = "'false'"!][!/*
  */!][!VAR "TcpIpCtrlIdx" = "0"!][!/*
    */!][!LOOP "node:order(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpCtrl/*, 'as:ref(TcpIpEthIfCtrlRef)/EthIfCtrlIdx')"!][!/*
      */!][!IF "num:i($x) = as:ref(TcpIpEthIfCtrlRef)/EthIfCtrlIdx"!][!/*
        */!][!VAR "referenced" = "'true'"!][!/*
        */!][!VAR "EthIfCtrlIdx2TcpIpCtrlIdx" = "concat($EthIfCtrlIdx2TcpIpCtrlIdx,' ',num:i($TcpIpCtrlIdx))"!][!/*
        */!][!VAR "TcpIpCtrlIdx2EthIfCtrlIdx" = "concat($TcpIpCtrlIdx2EthIfCtrlIdx,' ',num:i($x))"!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
      */!][!VAR "TcpIpCtrlIdx" = "num:i($TcpIpCtrlIdx + 1)"!][!/*
    */!][!ENDLOOP!][!/*
    */!][!IF "$referenced = 'false'"!][!/*
      */!][!VAR "EthIfCtrlIdx2TcpIpCtrlIdx" = "concat($EthIfCtrlIdx2TcpIpCtrlIdx,' ',num:i(255))"!][!/*
    */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*

*/!][!VAR "IpV4ArpDadCount" = "num:i(0)"!][!/*
*/!][!IF "((TcpIpGeneral/TcpIpIpV4General/TcpIpAutoIpEnabled = 'true') or (TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true'))"!][!/*
*/!][!VAR "IntArpDadIdx2CtrlId" = "''"!][!/*
*/!][!VAR "IpV4AutoIpLocalAddrId2ArpDadIdx" = "''"!][!/*
*/!][!VAR "IpV4AutoIpDoIpLocalAddrId2ArpDadIdx" = "''"!][!/*
*/!][!VAR "IpV4DhcpLocalAddrId2ArpDadIdx" = "''"!][!/*
*/!][!VAR "NumLocalAddr"!][!CALL "LocalAddrList_getNumLocalAddr"!][!ENDVAR!][!/*
*/!][!FOR "localAddrId" = "0" TO "$NumLocalAddr - 1"!][!/*
  */!][!VAR "localAddrDomain"!][!CALL "LocalAddrList_getDomain", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!VAR "addrType"!][!CALL "LocalAddrList_getAddrType", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
  */!][!IF "($localAddrDomain = 'TCPIP_AF_INET') and ($addrType = 'TCPIP_UNICAST')"!][!/*
    */!][!VAR "linkLocalMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL'"!][!ENDVAR!][!/*
    */!][!VAR "doipMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_LINKLOCAL_DOIP'"!][!ENDVAR!][!/*
    */!][!VAR "dhcpMethod"!][!CALL "LocalAddrList_getMethodOfMethod", "LocalAddrId"="$localAddrId", "method"="'TCPIP_DHCP'"!][!ENDVAR!][!/*
    */!][!VAR "idx"!][!CALL "LocalAddrList_getCtrlIdx", "LocalAddrId"="$localAddrId"!][!ENDVAR!][!/*
    */!][!IF "($linkLocalMethod = 'TCPIP_LINKLOCAL')"!][!/*
      */!][!VAR "IpV4AutoIpLocalAddrId2ArpDadIdx" = "concat($IpV4AutoIpLocalAddrId2ArpDadIdx,' ',num:i($IpV4ArpDadCount))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV4AutoIpLocalAddrId2ArpDadIdx" = "concat($IpV4AutoIpLocalAddrId2ArpDadIdx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
    */!][!IF "($doipMethod = 'TCPIP_LINKLOCAL_DOIP')"!][!/*
      */!][!VAR "IpV4AutoIpDoIpLocalAddrId2ArpDadIdx" = "concat($IpV4AutoIpDoIpLocalAddrId2ArpDadIdx,' ',num:i($IpV4ArpDadCount))"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV4AutoIpDoIpLocalAddrId2ArpDadIdx" = "concat($IpV4AutoIpDoIpLocalAddrId2ArpDadIdx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
    */!][!IF "($linkLocalMethod = 'TCPIP_LINKLOCAL') or ($doipMethod = 'TCPIP_LINKLOCAL_DOIP')"!][!/*
      */!][!VAR "IntArpDadIdx2CtrlId" = "concat($IntArpDadIdx2CtrlId,' ',num:i(text:split($EthIfCtrlIdx2TcpIpCtrlIdx,' ')[num:i($idx + 1)]))"!][!/*
      */!][!VAR "IpV4ArpDadCount" = "num:i($IpV4ArpDadCount + 1)"!][!/*
    */!][!ENDIF!][!/*
    */!][!IF "($dhcpMethod = 'TCPIP_DHCP') and (TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpArpProbingEnabled = 'true')"!][!/*
      */!][!VAR "IpV4DhcpLocalAddrId2ArpDadIdx" = "concat($IpV4DhcpLocalAddrId2ArpDadIdx,' ',num:i($IpV4ArpDadCount))"!][!/*
      */!][!VAR "IntArpDadIdx2CtrlId" = "concat($IntArpDadIdx2CtrlId,' ',num:i(text:split($EthIfCtrlIdx2TcpIpCtrlIdx,' ')[num:i($idx + 1)]))"!][!/*
      */!][!VAR "IpV4ArpDadCount" = "num:i($IpV4ArpDadCount + 1)"!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "IpV4DhcpLocalAddrId2ArpDadIdx" = "concat($IpV4DhcpLocalAddrId2ArpDadIdx,' NONE')"!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "IpV4AutoIpLocalAddrId2ArpDadIdx" = "concat($IpV4AutoIpLocalAddrId2ArpDadIdx,' NONE')"!][!/*
    */!][!VAR "IpV4AutoIpDoIpLocalAddrId2ArpDadIdx" = "concat($IpV4AutoIpDoIpLocalAddrId2ArpDadIdx,' NONE')"!][!/*
    */!][!VAR "IpV4DhcpLocalAddrId2ArpDadIdx" = "concat($IpV4DhcpLocalAddrId2ArpDadIdx,' NONE')"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!ENDIF!][!/*

Calculate number of TCP option filters
*/!][!VAR "NumOfTcpOptionFilters" = "0"!][!/*
*/!][!VAR "NumOfTcpFilterEntries" = "0"!][!/*
*/!][!VAR "TcpOptionFilterEntries" = "''"!][!/*
*/!][!VAR "TcpOptionFiltersOffset" = "''"!][!/*
*/!][!VAR "TcpOptionFilterNumPerId" = "''"!][!/*
*/!][!LOOP "node:order(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpTcpConfig/TcpIpTcpConfigOptionFilter/*, 'TcpIpTcpConfigOptionFilterId')"!][!/*
  */!][!VAR "NumOfTcpOptionFilters" = "num:i($NumOfTcpOptionFilters + 1)"!][!/*
  */!][!VAR "TcpOptionFiltersOffset"!][!"$TcpOptionFiltersOffset"!][!"num:i(count(text:split($TcpOptionFilterEntries, ';')))"!];[!ENDVAR!][!/*
  */!][!VAR "TcpOptionFilterNumPerId"!][!"$TcpOptionFilterNumPerId"!][!"num:i(count(./TcpIpTcpConfigOptionFilterEntry/*))"!];[!ENDVAR!][!/*
  */!][!LOOP "(./TcpIpTcpConfigOptionFilterEntry/*)"!][!/*
     */!][!VAR "TcpOptionFilterEntries"!][!"$TcpOptionFilterEntries"!][!"num:i(.)"!];[!ENDVAR!][!/*
  */!][!ENDLOOP!][!/*
*/!][!ENDLOOP!][!/*

*/!][!VAR "NumOfTcpFilterEntries" = "num:i(count(text:split($TcpOptionFilterEntries, ';')))"!][!/*

Calculate number of extension header filters
*/!][!VAR "ExtensionHdrIdList" = "''"!][!/*
*/!][!VAR "ExtensHdrIdCount" = "''"!][!/*
*/!][!LOOP "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpIpV6ConfigExtHeaderFilter/*/TcpIpIpV6ConfigExtHeaderFilterEntry/*)"!][!/*
  */!][!VAR "ExtensionHdrIdList"!][!"$ExtensionHdrIdList"!][!"num:i(.)"!];[!ENDVAR!][!/*
*/!][!ENDLOOP!][!/*
*/!][!VAR "ExtensHdrIdCount" = "num:i(count(text:split($ExtensionHdrIdList, ';')))"!][!/*

Calculate number of Dhcp option
*/!][!VAR "NumOfDhcpOptions" = "0"!][!/*
*/!][!VAR "DhcpParamReqOptionIdx" = "255"!][!/*
*/!][!VAR "DhcpOptionsTotalLen" = "0"!][!/*
*/!][!VAR "DhcpOptionOffset" = "''"!][!/*
*/!][!VAR "DhcpOptionCode" = "''"!][!/*
*/!][!VAR "DhcpOptionMaxLen" = "''"!][!/*
*/!][!VAR "DhcpOptionTransmit" = "''"!][!/*
*/!][!LOOP "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpConfig/TcpIpIpV4Config/TcpIpDhcpConfig/*[1]/TcpIpDhcpConfigurableOptions/*)"!][!/*
  */!][!VAR "NumOfDhcpOptions" = "num:i($NumOfDhcpOptions + 1)"!][!/*
  */!][!VAR "DhcpOptionOffset"!][!"$DhcpOptionOffset"!][!"num:i($DhcpOptionsTotalLen)"!];[!ENDVAR!][!/*
  */!][!VAR "DhcpOptionsTotalLen" = "num:i($DhcpOptionsTotalLen + num:i(./TcpIpDhcpOptionMaxLength))"!][!/*
  */!][!VAR "DhcpOptionCode"!][!"$DhcpOptionCode"!][!"num:i(./TcpIpDhcpOptionCode)"!];[!ENDVAR!][!/*
  */!][!VAR "DhcpOptionMaxLen"!][!"$DhcpOptionMaxLen"!][!"num:i(./TcpIpDhcpOptionMaxLength)"!];[!ENDVAR!][!/*
  */!][!VAR "DhcpOptionTransmit"!][!"$DhcpOptionTransmit"!][!IF "./TcpIpDhcpOptionTransmit = 'true'"!][!"num:i("1")"!][!ELSE!][!"num:i("0")"!][!ENDIF!];[!ENDVAR!][!/*
  */!][!IF "num:i(./TcpIpDhcpOptionCode) = 55"!][!/*
    */!][!VAR "DhcpParamReqOptionIdx" = "node:pos(.)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*

Calculate number of DhcpV6 option
*/!][!VAR "NumOfDhcpV6Options" = "0"!][!/*
*/!][!VAR "DhcpV6OptionsTotalLen" = "0"!][!/*
*/!][!VAR "DhcpV6OptionOffset" = "''"!][!/*
*/!][!VAR "DhcpV6OptionCode" = "''"!][!/*
*/!][!VAR "DhcpV6OptionMaxLen" = "''"!][!/*
*/!][!VAR "DhcpV6OptionTransmit" = "''"!][!/*
*/!][!LOOP "(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpConfig/TcpIpIpV6Config/TcpIpDhcpV6Config/*[1]/TcpIpDhcpV6ConfigurableOptions/*)"!][!/*
  */!][!VAR "NumOfDhcpV6Options" = "num:i($NumOfDhcpV6Options + 1)"!][!/*
  */!][!VAR "DhcpV6OptionOffset"!][!"$DhcpV6OptionOffset"!][!"num:i($DhcpV6OptionsTotalLen)"!];[!ENDVAR!][!/*
  */!][!VAR "DhcpV6OptionsTotalLen" = "num:i($DhcpV6OptionsTotalLen + num:i(./TcpIpDhcpV6OptionMaxLength))"!][!/*
  */!][!VAR "DhcpV6OptionCode"!][!"$DhcpV6OptionCode"!][!"num:i(./TcpIpDhcpV6OptionCode)"!];[!ENDVAR!][!/*
  */!][!VAR "DhcpV6OptionMaxLen"!][!"$DhcpV6OptionMaxLen"!][!"num:i(./TcpIpDhcpV6OptionMaxLength)"!];[!ENDVAR!][!/*
  */!][!VAR "DhcpV6OptionTransmit"!][!"$DhcpV6OptionTransmit"!][!IF "./TcpIpDhcpV6OptionTransmit = 'true'"!][!"num:i("1")"!][!ELSE!][!"num:i("0")"!][!ENDIF!];[!ENDVAR!][!/*
*/!][!ENDLOOP!][!/*

*/!][!VAR "numOfMemoryPools"="num:i(count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpMemoryConfig/TcpIpMemoryPool/*))"!][!/*

*/!][!MACRO "upperLayerToValue", "upperLayer", "domain"!][!/*
*/!][!IF "$upperLayer = 'UDP'"!][!/*
  */!][!VAR "result"="17"!][!/*
*/!][!ELSEIF "$upperLayer = 'TCP'"!][!/*
  */!][!VAR "result"="6"!][!/*
*/!][!ELSEIF "$upperLayer = 'ICMP'"!][!/*
  */!][!IF "$domain = 'TCPIP_AF_INET'"!][!/*
    */!][!VAR "result"="1"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "result"="58"!][!/*
  */!][!ENDIF!][!/*
*/!][!ELSEIF "$upperLayer = 'ANY'"!][!/*
  */!][!VAR "result"="0"!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i($result)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!VAR "numOfLocalAddr"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpLocalAddr/*)"!][!/*
*/!][!VAR "numOfIpSecConnections"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*)"!][!/*
*/!][!VAR "numOfIpv4IpSecConnections"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[TcpIpIpSecDomainType = 'TCPIP_AF_INET'])"!][!/*
*/!][!VAR "numOfIpv6IpSecConnections"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[TcpIpIpSecDomainType = 'TCPIP_AF_INET6'])"!][!/*
*/!][!VAR "numOfAddrRangeIpSecConnections"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[node:exists(TcpIpIpSecRemotePhysAddr/TcpIpIpSecStartRemotePhysAddr)])"!][!/*
*/!][!VAR "numOfSecurityPolicies"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*/TcpIpIpIpSecSecurityPolicies/*)"!][!/*
*/!][!VAR "numOfSecurityRules"="count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*/TcpIpIpIpSecSecurityPolicies/*/TcpIpIpIpSecSecurityRule/*)"!][!/*

*/!][!VAR "numOfSecurityAssociationConfigs"="num:i(count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecSecurityAssociationCfg/*))"!][!/*
*/!][!VAR "numOfIPsecProposals"="num:i(count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecSecurityAssociationCfg/*/TcpIpIpSecSecurityProposal/*))"!][!/*
*/!][!VAR "numOfSecurityAssociations"="num:i(count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecSecurityAssociationCfg/*/TcpIpIpSecSecurityProposal/*/TcpIpIpSecSecurityAssociation/*))"!][!/*

*/!][!MACRO "algorithmSupportsAead", "algorithm"!][!/*
*/!][!VAR "result"="0"!][!/*
*/!][!IF "$algorithm = 'ENCR_AES_GCM_256_ICV_12_No19'"!][!/*
  */!][!VAR "result"="1"!][!/*
*/!][!ELSEIF "$algorithm = 'ENCR_AES_GCM_256_ICV_16_No20'"!][!/*
  */!][!VAR "result"="1"!][!/*
*/!][!ELSEIF "$algorithm = 'AES_GMAC_128'"!][!/*
  */!][!VAR "result"="1"!][!/*
*/!][!ELSEIF "$algorithm = 'AES_GMAC_256'"!][!/*
  */!][!VAR "result"="1"!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i($result)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "getKeyTruncatedLengthForAlgorithm", "algorithm"!][!/*
*/!][!IF "$algorithm = 'AES_CMAC_96'"!][!/*
  */!][!VAR "result"="12"!][!/*
*/!][!ELSEIF "$algorithm = 'AES_GMAC_128'"!][!/*
  */!][!VAR "result"="16"!][!/*
*/!][!ELSEIF "$algorithm = 'AES_GMAC_256'"!][!/*
  */!][!VAR "result"="16"!][!/*
*/!][!ELSEIF "$algorithm = 'HMAC_SHA2_256_128'"!][!/*
  */!][!VAR "result"="16"!][!/*
*/!][!ELSEIF "$algorithm = 'AUTH_HMAC_SHA2_384_192_No13'"!][!/*
  */!][!VAR "result"="24"!][!/*
*/!][!ELSEIF "$algorithm = 'AUTH_HMAC_SHA2_256_128_No12'"!][!/*
  */!][!VAR "result"="16"!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i($result)"!][!/*
*/!][!ENDMACRO!][!/*

* get the length of AH (IPv4 32 bit multiple, IPv6 64 bit multiple)
*/!][!MACRO "getAHSizeForAlgorithm", "algorithm", "domain"!][!/*
*/!][!IF "$algorithm = 'AES_CMAC_96'"!][!/*
  */!][!VAR "result"="24"!][!/*
*/!][!ELSEIF "$algorithm = 'AES_GMAC_128'"!][!/*
  */!][!IF "$domain = 'TCPIP_AF_INET'"!][!/*
    */!][!VAR "result"="36"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "result"="40"!][!/*
  */!][!ENDIF!][!/*
*/!][!ELSEIF "$algorithm = 'AES_GMAC_256'"!][!/*
  */!][!IF "$domain = 'TCPIP_AF_INET'"!][!/*
    */!][!VAR "result"="36"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "result"="40"!][!/*
  */!][!ENDIF!][!/*
*/!][!ELSEIF "$algorithm = 'HMAC_SHA2_256_128'"!][!/*
  */!][!IF "$domain = 'TCPIP_AF_INET'"!][!/*
    */!][!VAR "result"="28"!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "result"="32"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDIF!][!/*
*/!][!"num:i($result)"!][!/*
*/!][!ENDMACRO!][!/*

*/!][!MACRO "setIpSecRemoteAddresses", "domain"!][!/*
*/!][!LOOP "node:order(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*, 'as:ref(TcpIpIpSecLocalAddrRef)/TcpIpAddrId', 'IpSecRemoteAddrConfig = "AddressRange"')"!][!/*
  */!][!IF "./TcpIpIpSecDomainType = $domain"!][!/*
    */!][!IF "IpSecRemoteAddrConfig = 'SingleAddress'"!][!/*
      */!][!VAR "addresses"!][!"IpSecRemoteAddrConfig/TcpIpIpSecRemoteAddr"!][!ENDVAR!][!/*
    */!][!ELSE!][!/*
      */!][!VAR "addresses"!][!"IpSecRemoteAddrConfig/TcpIpIpSecStartRemoteAddr"!];[!"IpSecRemoteAddrConfig/TcpIpIpSecEndRemoteAddr"!][!ENDVAR!][!/*
    */!][!ENDIF!][!/*
    */!][!FOR "x" = "1" TO "count(text:split($addresses,';'))"!][!/*
       */!][!VAR "ipAddr"="text:split($addresses,';')[num:i($x)]"!][!//
           <field>
             <field>
             [!IF "./TcpIpIpSecDomainType = 'TCPIP_AF_INET'"!]
               <field>
                 <int>[!CALL "IpV4AddressToUINT32", "IpAddress" = "$ipAddr", "endianCheck"="'true'"!]</int> <!-- remote IPv4 address -->
               </field>
             [!ELSE!]
               [!CALL "IpV6AddressToUINT32", "IpAddress" = "$ipAddr"!] <!-- remote IPv6 address -->
             [!ENDIF!]
             </field>
            </field>
       [!ENDFOR!][!/*
   */!][!ENDIF!][!/*
 */!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*

*/!][!VAR "numOfV4BypassOnly" = "count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[count(./TcpIpIpIpSecSecurityPolicies/*[TcpIpIpSecSecurityPolicyMechanism = 'SECURED'] = 0) and (TcpIpIpSecDomainType = 'TCPIP_AF_INET')])"!][!/*
*/!][!VAR "numOfV6BypassOnly" = "count(as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[count(./TcpIpIpIpSecSecurityPolicies/*[TcpIpIpSecSecurityPolicyMechanism = 'SECURED'] = 0) and (TcpIpIpSecDomainType = 'TCPIP_AF_INET6')])"!][!/*
*/!][!IF "(TcpIpGeneral/TcpIpSecurityMode != 'NO_SECURITY')"!][!/*

*/!][!VAR "errorLog" = "''"!][!/*
*/!][!LOOP "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[IpSecRemoteAddrConfig = 'AddressRange']"!][!/*
*/!][!VAR "valuesOk" = "'false'"!][!/*
*/!][!VAR "startAddr" = "num:i(1)"!][!/*
*/!][!VAR "endAddr" = "num:i(0)"!][!/*
  */!][!IF "TcpIpIpSecDomainType = 'TCPIP_AF_INET'"!][!/*
    */!][!VAR "startAddr"!][!CALL "IpV4AddressToUINT32", "IpAddress"="IpSecRemoteAddrConfig/TcpIpIpSecStartRemoteAddr", "endianCheck"="'false'"!][!ENDVAR!][!/*
   */!] [!VAR "endAddr"!][!CALL "IpV4AddressToUINT32", "IpAddress"="IpSecRemoteAddrConfig/TcpIpIpSecEndRemoteAddr", "endianCheck"="'false'"!][!ENDVAR!][!/*
    */!][!IF "num:i($startAddr) <= num:i($endAddr)"!][!/*
      */!][!VAR "valuesOk" = "'true'"!][!/*
    */!][!ENDIF!][!/*
  */!][!ELSE!][!/*
    */!][!VAR "startAddr"!][!CALL "IpV6AddressToUINT32List", "IpAddress" = "IpSecRemoteAddrConfig/TcpIpIpSecStartRemoteAddr"!][!ENDVAR!][!/*
    */!][!VAR "endAddr"!][!CALL "IpV6AddressToUINT32List", "IpAddress" = "IpSecRemoteAddrConfig/TcpIpIpSecEndRemoteAddr"!][!ENDVAR!][!/*
    */!][!FOR "x"="1" TO "num:i(4)"!][!/*
      */!][!IF "(num:i(text:split($startAddr, ' '))[num:i($x)]) < (num:i(text:split($endAddr, ' '))[num:i($x)])"!][!/*
        */!][!VAR "valuesOk" = "'true'"!][!/*
        */!][!BREAK!][!/*
      */!][!ELSEIF "(num:i(text:split($startAddr, ' '))[num:i($x)]) = (num:i(text:split($endAddr, ' '))[num:i($x)])"!][!/*
        */!][!VAR "valuesOk" = "'true'"!][!/*
      */!][!ELSEIF "(num:i(text:split($startAddr, ' '))[num:i($x)]) > (num:i(text:split($endAddr, ' '))[num:i($x)])"!][!/*
        */!][!VAR "valuesOk" = "'false'"!][!/*
        */!][!BREAK!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDFOR!][!/*
  */!][!ENDIF!][!/*
[!/**/!][!IF "$valuesOk = 'false'"!][!/*
  */!][!VAR "errorLog" = "concat($errorLog, ' ', node:name(.))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "$errorLog != ''"!][!/*
  */!][!ERROR!]Start remote address is greater than the end remote address for the following bypass only IpSec connections: [!"$errorLog"!][!ENDERROR!][!/*
*/!][!ENDIF!][!/*

*/!][!LOOP "as:modconf('TcpIp')[1]/TcpIpConfig/TcpIpIpSecConfig/TcpIpIpSecConnections/*[node:exists(TcpIpIpSecRemotePhysAddr/TcpIpIpSecStartRemotePhysAddr)]"!][!/*
*/!][!VAR "valuesOk" = "'false'"!][!/*
*/!][!VAR "startAddrH" = "num:i(1)"!][!/*
*/!][!VAR "startAddrL" = "num:i(1)"!][!/*
*/!][!VAR "endAddrH" = "num:i(0)"!][!/*
*/!][!VAR "endAddrL" = "num:i(0)"!][!/*
*/!]    [!VAR "startAddrH"!][!CALL "MACAddressToUINT32H", "MacAddress"="./TcpIpIpSecRemotePhysAddr/TcpIpIpSecStartRemotePhysAddr", "StartId"="1"!][!ENDVAR!][!/*
*/!]    [!VAR "startAddrL"!][!CALL "MACAddressToUINT32H", "MacAddress"="./TcpIpIpSecRemotePhysAddr/TcpIpIpSecStartRemotePhysAddr", "StartId"="4"!][!ENDVAR!][!/*
*/!]    [!VAR "endAddrH"!][!CALL "MACAddressToUINT32H", "MacAddress"="./TcpIpIpSecRemotePhysAddr/TcpIpIpSecEndRemotePhysAddr", "StartId"="1"!][!ENDVAR!][!/*
*/!]    [!VAR "endAddrL"!][!CALL "MACAddressToUINT32H", "MacAddress"="./TcpIpIpSecRemotePhysAddr/TcpIpIpSecEndRemotePhysAddr", "StartId"="4"!][!ENDVAR!][!/*
*/!]    [!IF "(num:i($endAddrH) < num:i($startAddrH)) or ((num:i($endAddrH) = num:i($startAddrH)) and (num:i($endAddrL) < num:i($startAddrL)))"!][!/*
*/!]      [!VAR "valuesOk" = "'false'"!][!/*
*/!]    [!ELSE!][!/*
*/!]      [!VAR "valuesOk" = "'true'"!][!/*
*/!]    [!ENDIF!][!/*
*/!][!IF "$valuesOk = 'false'"!][!/*
*/!]  [!VAR "errorLog" = "concat($errorLog, ' ', node:name(.))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDLOOP!][!/*
*/!][!IF "$errorLog != ''"!][!/*
*/!][!ERROR!]Start remote MAC address is greater than the end remote MAC address for the following connections: [!"$errorLog"!][!ENDERROR!][!/*
*/!][!ENDIF!][!/*

*/!][!ENDIF!][!/*

*/!][!ENDSELECT!][!//
