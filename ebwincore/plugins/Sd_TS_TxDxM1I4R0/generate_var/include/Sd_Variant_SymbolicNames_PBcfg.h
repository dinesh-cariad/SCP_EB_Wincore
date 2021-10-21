/**
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
 */

[!CODE!]
[!AUTOSPACING!]

[!IF "var:defined('postBuildVariant')"!]
  [!/* Current PB variant Sufix
  */!][!VAR "SD_VARIANT_SUFIX"="concat('_',$postBuildVariant)"!]

#ifndef SD_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
#define SD_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H
[!ELSE!]
  [!/* Current PB variant Sufix
  */!][!VAR "SD_VARIANT_SUFIX"="string("")"!]
#ifndef SD_SYMBOLICNAMES_PBCFG_H
#define SD_SYMBOLICNAMES_PBCFG_H
[!ENDIF!]

/*==================[includes]==============================================*/

/*==================[macros]================================================*/

[!SELECT "SdConfig/*[1]"!][!//

[!LOOP "as:modconf('Sd')[1]/SdConfig/*[1]/SdInstance/*/SdClientService/*"!][!//
#define SD_CLIENT_SERVICE_[!"name(../..)"!]_[!"name(.)"!][!"$SD_VARIANT_SUFIX"!] [!"num:i(SdClientServiceHandleId)"!]U
[!ENDLOOP!][!//

[!LOOP "SdInstance/*/SdClientService/*/SdConsumedEventGroup/*"!][!//
#define SD_CONSUMED_EVENTGROUP_[!"name(../../../..)"!]_[!"name(../..)"!]_[!"name(.)"!][!"$SD_VARIANT_SUFIX"!] [!"num:i(SdConsumedEventGroupHandleId)"!]U
[!ENDLOOP!][!//

[!LOOP "SdInstance/*/SdServerService/*"!][!//
#define SD_SERVER_SERVICE_[!"name(../..)"!]_[!"name(.)"!][!"$SD_VARIANT_SUFIX"!] [!"num:i(SdServerServiceHandleId)"!]U
[!ENDLOOP!][!//

[!VAR "EventHandlerRefCount"!][!"num:i(0)"!][!ENDVAR!][!//
[!LOOP "SdInstance/*/SdServerService/*/SdEventHandler/*"!][!//
#define SD_EVENT_HANDLER_[!"name(../../../..)"!]_[!"name(../..)"!]_[!"name(.)"!][!"$SD_VARIANT_SUFIX"!] [!"num:i(SdEventHandlerHandleId)"!]U
[!IF "node:refvalid(../../SdServerServiceUdpRef)"!][!//
[!VAR "EventHandlerRefCount"!][!"num:i($EventHandlerRefCount + count(as:ref(../../SdServerServiceUdpRef)/SoAdSocketConnection/*))"!][!ENDVAR!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

#define SD_SERVER_SERVICE_EVENTREF_COUNT[!"$SD_VARIANT_SUFIX"!] [!"num:i($EventHandlerRefCount)"!]U

#define SD_MAXNUM_SUBSCRIPTIONS_REMOTENODE[!"$SD_VARIANT_SUFIX"!] 16U

/* Count of all SoConGroups and SoCons used by the server services (Udp + Tcp) */
[!VAR "ServerSoConCount"!][!"num:i(0)"!][!ENDVAR!][!//
[!VAR "ServerSoConGroupCount"!][!"num:i(0)"!][!ENDVAR!][!//

[!VAR "references" = "text:join(as:modconf('Sd')/SdConfig/*[1]/SdInstance/*/SdServerService/*/SdServerServiceUdpRef|as:modconf('Sd')/SdConfig/*[1]/SdInstance/*/SdServerService/*/SdServerServiceTcpRef|as:modconf('Sd')/SdConfig/*[1]/SdInstance/*/SdClientService/*/SdClientServiceUdpRef|as:modconf('Sd')/SdConfig/*[1]/SdInstance/*/SdClientService/*/SdClientServiceTcpRef)"!][!//
[!LOOP "as:modconf('SoAd')/SoAdConfig/*[1]/SoAdSocketConnectionGroup/*[text:contains(string($references),as:path(node:path(.)))]"!][!//
[!VAR "ServerSoConCount" = "num:i($ServerSoConCount + count(SoAdSocketConnection/*))"!][!//
[!VAR "ServerSoConGroupCount" = "num:i($ServerSoConGroupCount + 1)"!][!//
[!ENDLOOP!][!//
#define SD_SERVER_SOCON_COUNT[!"$SD_VARIANT_SUFIX"!] [!"num:i($ServerSoConCount)"!]U
#define SD_SERVER_SOCONGROUP_COUNT[!"$SD_VARIANT_SUFIX"!] [!"num:i($ServerSoConGroupCount)"!]U

#define SD_CLIENT_TIMER_COUNT[!"$SD_VARIANT_SUFIX"!] [!"num:i(count(./SdInstance/*/SdClientTimer/*))"!]

#define SD_SERVER_TIMER_COUNT[!"$SD_VARIANT_SUFIX"!] [!"num:i(count(./SdInstance/*/SdServerTimer/*))"!]

[!VAR "ConfigurationStringLen"!][!"num:i(0)"!][!ENDVAR!][!//
[!VAR "ConfigStringsRefsCount"!][!"num:i(0)"!][!ENDVAR!][!//

[!//
[!LOOP "SdInstance/*/SdClientService/*"!][!//
[!VAR "ClientServiceConfigurationString" = "''"!][!//
[!IF "node:exists(../../SdInstanceHostname)"!][!//
[!VAR "ClientServiceConfigurationString" = "concat(' "\', substring(num:inttohex(string-length(../../SdInstanceHostname) + 9), 2), '""hostname=', node:value(../../SdInstanceHostname), '"')"!][!//
[!ENDIF!][!//
[!LOOP "SdClientCapabilityRecord/*"!][!//
[!IF "node:exists(./SdClientServiceCapabilityRecordValue)"!][!//
[!VAR "CapabilityRecord" = "concat(./SdClientServiceCapabilityRecordKey, '=', ./SdClientServiceCapabilityRecordValue)"!][!//
[!ELSE!][!//
[!VAR "CapabilityRecord" = "./SdClientServiceCapabilityRecordKey"!][!//
[!ENDIF!][!//
[!VAR "ClientServiceConfigurationString" = "concat($ClientServiceConfigurationString, ' "\', substring(num:inttohex(string-length($CapabilityRecord)), 2), '""', $CapabilityRecord, '"')"!][!//
[!ENDLOOP!][!//
[!//
[!IF "string-length($ClientServiceConfigurationString) > 0"!][!//
[!VAR "ConfigurationStringLen"!][!"num:i($ConfigurationStringLen + string-length($ClientServiceConfigurationString))"!][!ENDVAR!][!//
[!VAR "ConfigStringsRefsCount"!][!"num:i($ConfigStringsRefsCount + 1)"!][!ENDVAR!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

[!LOOP "SdInstance/*/SdServerService/*"!][!//
[!VAR "ServerServiceConfigurationString" = "''"!][!//
[!IF "node:exists(../../SdInstanceHostname)"!][!//
[!VAR "ServerServiceConfigurationString" = "concat(' "\', substring(num:inttohex(string-length(../../SdInstanceHostname) + 9), 2), '""hostname=', node:value(../../SdInstanceHostname), '"')"!][!//
[!ENDIF!][!//
[!LOOP "SdServerCapabilityRecord/*"!][!//
[!IF "node:exists(./SdServerCapabilityRecordValue)"!][!//
[!VAR "CapabilityRecord" = "concat(./SdServerCapabilityRecordKey, '=', ./SdServerCapabilityRecordValue)"!][!//
[!ELSE!][!//
[!VAR "CapabilityRecord" = "./SdServerCapabilityRecordKey"!][!//
[!ENDIF!][!//
[!VAR "ServerServiceConfigurationString" = "concat($ServerServiceConfigurationString, ' "\', substring(num:inttohex(string-length($CapabilityRecord)), 2), '""', $CapabilityRecord, '"')"!][!//
[!ENDLOOP!][!//
[!//
[!IF "string-length($ServerServiceConfigurationString) > 0"!][!//
[!VAR "ConfigurationStringLen"!][!"num:i($ConfigurationStringLen + string-length($ServerServiceConfigurationString))"!][!ENDVAR!][!//
[!VAR "ConfigStringsRefsCount"!][!"num:i($ConfigStringsRefsCount + 1)"!][!ENDVAR!][!//
[!ENDIF!][!//
[!ENDLOOP!][!//

#define SD_CONFIG_STRING_INDEX_COUNT[!"$SD_VARIANT_SUFIX"!] [!"num:i($ConfigStringsRefsCount)"!]U
#define SD_CONFIG_STRING_LENGTH[!"$SD_VARIANT_SUFIX"!] [!"num:i($ConfigurationStringLen)"!]U

[!ENDSELECT!][!//

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]==========================*/
[!IF "var:defined('postBuildVariant')"!]
#endif /* ifndef SD_SYMBOLICNAMES[!"$postBuildVariant"!]_PBCFG_H */
[!ELSE!]
#endif /* ifndef SD_SYMBOLICNAMES_PBCFG_H */
[!ENDIF!]
/*==================[end of file]============================================*/
[!ENDCODE!]
