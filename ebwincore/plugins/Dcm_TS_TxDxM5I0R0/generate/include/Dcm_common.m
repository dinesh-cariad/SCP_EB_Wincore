[!/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X10"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x10')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X11"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x11')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X14"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x14')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X22"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x22')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X23"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x23')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X24"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x24')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X27"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x27')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X28"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x28')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X2A"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x2A')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X2C"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x2C')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X2E"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x2E')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X2F"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x2F')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X31"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x31')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X34"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x34')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X35"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x35')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X36"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x36')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X37"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x37')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X3D"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x3D')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X3E"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x3E')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X86"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x86')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X87"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x87')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03"!][!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = num:hextoint('0x86')]/*/*[(DcmDsdSubServiceId = num:hextoint('0x03')) and (not(node:exists(DcmDsdSubServiceFnc)))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43"!][!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = num:hextoint('0x86')]/*/*[(DcmDsdSubServiceId = num:hextoint('0x43')) and (not(node:exists(DcmDsdSubServiceFnc)))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SUBSERVICE_DDDID_0x02"!][!IF "node:exists(DcmConfigSet/*/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[DcmDsdSidTabServiceId = num:hextoint('0x2C')]/*/*[(DcmDsdSubServiceId = num:hextoint('0x02')) and (not(node:exists(DcmDsdSubServiceFnc)))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_MODEDECLARATION_SUPPORT"!][!IF "DcmConfigSet/*[1]/DcmGeneral/DcmModeDeclarationSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X19"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x19')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0XAF"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0xAF')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X01"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x01')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X02"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x02')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X03"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x03')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X04"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x04')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X06"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x06')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X07"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x07')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X08"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x08')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X09"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x09')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X0A"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x0A')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_SERVICE_0X85"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmDsdSidTabServiceId = num:hextoint('0x85')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SERVICE_0X06_ASYNC"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmAsyncServiceExecution = 'true') and (DcmDsdSidTabServiceId = num:hextoint('0x06')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SERVICE_0X22_ASYNC"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmAsyncServiceExecution = 'true') and (DcmDsdSidTabServiceId = num:hextoint('0x22')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SERVICE_0X2E_ASYNC"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmAsyncServiceExecution = 'true') and (DcmDsdSidTabServiceId = num:hextoint('0x2E')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_SERVICE_0X2F_ASYNC"!][!IF "node:exists(DcmConfigSet/*[1]/DcmDsd/DcmDsdServiceTable/*/DcmDsdService/*[((DcmAsyncServiceExecution = 'true') and (DcmDsdSidTabServiceId = num:hextoint('0x2F')) and ((not(node:exists(DcmDsdSidTabFnc))) or ((node:exists(DcmDsdSidTabFnc)) and (DcmDsdDisableGenericServiceImplementation = 'false'))))])"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!VAR "DCM_DSP_USE_DEMCLIENTREF"!][!IF "node:exists(as:modconf('Dcm')[1]/DcmConfigSet/*[1]/DcmDsl/DcmDslProtocol/DcmDslProtocolRow/*/./DcmDemClientRef)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!][!ENDVAR!][!//
[!/* ---------- DCM_DSP_USE_DIDSERVICES */!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X22 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X24 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X2A = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X2C = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X2E = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X2F = 'STD_ON' or
      $DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x03 = 'STD_ON' or
      $DCM_DSP_SUBSERVICE_RESPONSEONEVENT_0x43 = 'STD_ON'"!][!//
[!VAR "DCM_DSP_USE_DIDSERVICES" = "'STD_ON'"!][!//
[!ELSE!][!//
[!VAR "DCM_DSP_USE_DIDSERVICES" = "'STD_OFF'"!][!//
[!ENDIF!][!//
[!/* ---------- DCM_DID_SIGNALS_COUNT */!][!//
[!VAR "DCM_DID_SIGNALS_COUNT"!][!"num:i(count(DcmConfigSet/*[1]/DcmDsp/DcmDspDid/*[DcmDspDidUsed = 'true']/DcmDspDidSignal/*))"!][!ENDVAR!][!//
[!/* ---------- DCM_DSP_USE_MEMORYSERVICES */!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X23 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X3D = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X34 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X35 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X2C = 'STD_ON'"!][!//
[!VAR "DCM_DSP_USE_MEMORYSERVICES" = "'STD_ON'"!][!//
[!ELSE!][!//
[!VAR "DCM_DSP_USE_MEMORYSERVICES" = "'STD_OFF'"!][!//
[!ENDIF!][!//
[!/* ---------- DCM_DSP_USE_DATATRANSFERSERVICES */!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X34 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X35 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X36 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X37 = 'STD_ON'"!][!//
[!VAR "DCM_DSP_USE_DATATRANSFERSERVICES" = "'STD_ON'"!][!//
[!ELSE!][!//
[!VAR "DCM_DSP_USE_DATATRANSFERSERVICES" = "'STD_OFF'"!][!//
[!ENDIF!][!//
[!/*---------- DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION */!][!//
[!IF "($DCM_DSP_USE_SERVICE_0X23 = 'STD_ON' or
       $DCM_DSP_USE_SERVICE_0X34 = 'STD_ON' or
       $DCM_DSP_USE_SERVICE_0X35 = 'STD_ON' or
       $DCM_DSP_USE_SERVICE_0X3D = 'STD_ON' or
       $DCM_DSP_SUBSERVICE_DDDID_0x02 = 'STD_ON') and
      node:exists(DcmConfigSet/*[1]/DcmDsp/DcmDspMemory/DcmDspAddressAndLengthFormatIdentifier)"!][!//
[!VAR "DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION" = "'STD_ON'"!][!//
[!ELSE!][!//
[!VAR "DCM_DSP_USE_ADDRESSLENGTHANDFORMAT_VERIFICATION" = "'STD_OFF'"!][!//
[!ENDIF!][!//
[!/*---------- DCM_DSP_USE_GETMEMORYADDRESSANDSIZE */!][!//
[!IF "$DCM_DSP_USE_SERVICE_0X23 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X34 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X35 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X3D = 'STD_ON' or
      $DCM_DSP_SUBSERVICE_DDDID_0x02 = 'STD_ON'"!][!//
[!VAR "DCM_DSP_USE_GETMEMORYADDRESSANDSIZE" = "'STD_ON'"!][!//
[!ELSE!][!//
[!VAR "DCM_DSP_USE_GETMEMORYADDRESSANDSIZE" = "'STD_OFF'"!][!//
[!ENDIF!][!//
[!/* ---------- DCM_DSP_USE_DEMFACADE */!][!//
[!IF "($DCM_DSP_USE_DEMCLIENTREF = 'STD_ON') and
      ($DCM_DSP_USE_SERVICE_0X19 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0XAF = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X02 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X03 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X06 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X07 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X0A = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X04 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X14 = 'STD_ON' or
      $DCM_DSP_USE_SERVICE_0X85 = 'STD_ON')"!][!//
[!VAR "DCM_DSP_USE_DEMFACADE" = "'STD_ON'"!][!//
[!ELSE!][!//
[!VAR "DCM_DSP_USE_DEMFACADE" = "'STD_OFF'"!][!//
[!ENDIF!][!//
[!/* --- Array for holding Shortnames; used for uniqueness check */!][!//
[!VAR "ShortNameList"="''"!][!//
[!/* MACRO to get the string for the symbolic name value.
     "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
     "OldName" must hold an unique string for the case that the container does not have a short name.
   */!][!//
[!MACRO "GetSymbolName","ShortNameRef","OldName"!][!//
[!INDENT "0"!][!//
  [!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!//
  [!IF "$SymbolName = ''"!][!//
    [!VAR "SymbolName" = "concat(name(../../.),'_',name(.),'_',$OldName)"!][!//
  [!ELSE!][!//
    [!VAR "ShortNameList" = "concat($ShortNameList,' ',$SymbolName)"!][!//
    [!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!][!//
      [!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$OldName"!] is not unique.[!ENDERROR!][!//
    [!ENDIF!][!//
    [!VAR "SymbolName" = "concat($OldName,'_',$SymbolName)"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
