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

[!/* *** multiple inclusion protection *** */!]
[!IF "not(var:defined('UDPNM_MACROS_M'))"!]
[!VAR "UDPNM_MACROS_M" = "'true'"!]

[!/*
*************************************************************************
* MACRO to get the string for the symbolic name value.
* "ShortNameRef" must reference the container holding the short name.
*************************************************************************
*/!]
[!NOCODE!][!//
[!MACRO "GetSymbolNameTx","ShortNameRef"!][!//
  [!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!//
  [!IF "$SymbolName = ''"!][!//
    [!VAR "SymbolName" = "concat('UdpNmConf_',name(../..),'_',name(..),'_',name(.))"!][!//
  [!ELSE!][!//
    [!VAR "SymbolName" = "concat('UdpNmConf_',name(.),'_',$SymbolName)"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!ENDNOCODE!]
[!/*
*************************************************************************
* MACRO to get the string for the symbolic name value.
* "ShortNameRef" must reference the container holding the short name.
*************************************************************************
*/!]
[!NOCODE!][!//
[!MACRO "GetSymbolNameRx","ShortNameRef"!][!//
  [!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!//
  [!IF "$SymbolName = ''"!][!//
    [!VAR "SymbolName" = "concat('UdpNmConf_',name(../../..),'_',name(../..),'_',name(.))"!][!//
  [!ELSE!][!//
    [!VAR "SymbolName" = "concat('UdpNmConf_',name(../../..),'_',name(../..),'_',$SymbolName)"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!ENDNOCODE!][!//

[!ENDIF!]