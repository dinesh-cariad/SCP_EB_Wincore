[!/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!AUTOSPACING!]

[!/* multiple inclusion protection */!]
[!IF "not(var:defined('NM_CFG_M'))"!]
[!VAR "NM_CFG_M"="'true'"!]

[!VAR "BusNmNum" = "0"!]
[!VAR "BusNmList" = "''"!]
[!VAR "BusNmListFullName" = "''"!]
[!VAR "BusNmPassiveMode" = "''"!]
[!/*
  CanNm Information
*/!]
[!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = 'NM_BUSNM_CANNM')]) > 0"!]
  [!VAR "CanNmEnabled" = "'true'"!]
  [!/* Internal ID of CanNm*/!]
  [!VAR "CanNmId"      = "$BusNmNum"!]
  [!VAR "BusNmList"    = "' CanNm'"!]
  [!VAR "BusNmListFullName"    = "' NM_BUSNM_CANNM'"!]
  [!VAR "BusNmNum"     = "$BusNmNum + 1"!]
  [!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = 'NM_BUSNM_CANNM') and (NmPassiveModeEnabled ='false')]) > 0"!]
    [!VAR "BusNmPassiveMode" =  "'false'"!]
  [!ELSE!]
    [!VAR "BusNmPassiveMode" =  "'true'"!]
  [!ENDIF!]
[!ELSE!]
  [!VAR "CanNmEnabled" = "'false'"!]
[!ENDIF!]

[!/*
  FrNm Information
*/!]
[!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = 'NM_BUSNM_FRNM')]) > 0"!]
  [!VAR "FrNmEnabled" = "'true'"!]
  [!/* Internal ID of FrNm*/!]
  [!VAR "FrNmId"      = "$BusNmNum"!]
  [!VAR "BusNmList"   = "concat($BusNmList,' FrNm')"!]
  [!VAR "BusNmListFullName"   = "concat($BusNmListFullName,' NM_BUSNM_FRNM')"!]
  [!VAR "BusNmNum"    = "$BusNmNum + 1"!]
  [!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = 'NM_BUSNM_FRNM') and (NmPassiveModeEnabled ='false')]) > 0"!]
    [!VAR "BusNmPassiveMode"   = "concat($BusNmPassiveMode,' false')"!]
  [!ELSE!]
    [!VAR "BusNmPassiveMode"   = "concat($BusNmPassiveMode,' true')"!]
  [!ENDIF!]
[!ELSE!]
  [!VAR "FrNmEnabled" = "'false'"!]
[!ENDIF!]

[!/*
  UdpNm Information
*/!]
[!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = 'NM_BUSNM_UDPNM')]) > 0"!]
  [!VAR "UdpNmEnabled" = "'true'"!]
  [!/* Internal ID of UdpNm*/!]
  [!VAR "UdpNmId"      = "$BusNmNum"!]
  [!VAR "BusNmList"   = "concat($BusNmList,' UdpNm')"!]
  [!VAR "BusNmListFullName"   = "concat($BusNmListFullName,' NM_BUSNM_UDPNM')"!]
  [!VAR "BusNmNum"    = "$BusNmNum + 1"!]
  [!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmStandardBusType)) and (NmBusType/NmStandardBusType = 'NM_BUSNM_UDPNM') and (NmPassiveModeEnabled ='false')]) > 0"!]
    [!VAR "BusNmPassiveMode"   = "concat($BusNmPassiveMode,' false')"!]
  [!ELSE!]
    [!VAR "BusNmPassiveMode"   = "concat($BusNmPassiveMode,' true')"!]
  [!ENDIF!]
[!ELSE!]
  [!VAR "UdpNmEnabled" = "'false'"!]
[!ENDIF!]

[!/*
  Generic Nm Information
*/!]
[!IF "node:exists(NmChannelConfig/*/NmBusType/NmGenericBusNmPrefix)"!]
  [!VAR "GenericNmEnabled" = "'true'"!]
/* Find the number of unique Generic Nms */

  [!VAR "NmPrefix" = "''"!]
  [!LOOP "NmChannelConfig/*"!]
    [!IF "node:exists(NmBusType/NmGenericBusNmPrefix)"!]
      [!VAR "NmPrefix" = "concat($NmPrefix, " ", NmBusType/NmGenericBusNmPrefix)"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!VAR "GenericNmId" = "''"!]
  [!VAR "GenericUniqNm" = "''"!]
/* Create BusNm List */
  [!FOR "BusId" = "1" TO "count(text:order(text:split($NmPrefix)))"!][!//
   [!/* Internal ID of Generic NMs*/!]
  [!VAR "GenericNmId"      ="concat($GenericNmId," ",$BusNmNum)"!]
  [!VAR "BusNmName" = "text:order(text:split($NmPrefix))[position() = $BusId]"!][!//
  [!VAR "BusNmList"   = "concat($BusNmList," ", $BusNmName)"!]
  [!VAR "BusNmListFullName"   = "concat($BusNmListFullName," ", $BusNmName)"!]
  [!VAR "GenericUniqNm" = "concat($GenericUniqNm," ", $BusNmName)"!]
  [!IF "count(NmChannelConfig/*[(node:exists(NmBusType/NmGenericBusNmPrefix)) and (NmBusType/NmGenericBusNmPrefix = $BusNmName) and (NmPassiveModeEnabled ='false')]) > 0"!]
    [!VAR "BusNmPassiveMode"   = "concat($BusNmPassiveMode,' false')"!]
  [!ELSE!]
    [!VAR "BusNmPassiveMode"   = "concat($BusNmPassiveMode,' true')"!]
  [!ENDIF!]
  [!VAR "BusNmNum"    = "$BusNmNum + 1"!]
[!ENDFOR!]
[!ELSE!]
  [!VAR "GenericNmEnabled" = "'false'"!]
[!ENDIF!]
[!/* end of multiple inclusion protection */!]
[!ENDIF!]

[!ENDNOCODE!]
