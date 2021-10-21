[!/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]

[!// Included by all generated files

[!IF "not(var:defined('FRTP_VARIABLES_M'))"!]
[!VAR "FRTP_VARIABLES_M"="'true'"!]

[!// EXPORTED VARIABLES ----------------------------------------------------------------------------

[!VAR "NumberOfConnections" = "num:i(count(FrTpMultipleConfig/*[1]/FrTpConnection/*))"!]
[!VAR "NumberOfTxConnections" = "num:i(count(FrTpMultipleConfig/*[1]/FrTpConnection/*/FrTpTxSdu/..))"!]
[!VAR "NumberOfTxPdus" = "num:i(count(FrTpMultipleConfig/*[1]/FrTpTxPduPool/*/FrTpTxPdu/*))"!]
[!VAR "FrTpMainFuncCycle" = "FrTpGeneral/FrTpMainFuncCycle"!]

[!IF "node:exists(FrTpGeneral/VendorSpecific/FrTpLimitNumberOfConnections)"!]
  [!VAR "FrTpLimitNumberOfConnections" = "FrTpGeneral/VendorSpecific/FrTpLimitNumberOfConnections"!]
  [!VAR "NumberOfConnectionLimits" = "num:i(count(FrTpGeneral/VendorSpecific/FrTpConnectionLimitConfig/*))"!]
  [!VAR "NumberOfConnectionBuffers" = "num:i($NumberOfConnectionLimits+1)"!]

  [!IF "$NumberOfConnectionLimits < 255"!]
    [!VAR "ConnectionLimitConfigIdxType" = "'uint8'"!]
  [!ELSEIF "$NumberOfConnectionLimits < 65535"!]
    [!VAR "ConnectionLimitConfigIdxType" = "'uint16'"!]
  [!ENDIF!]

  [!VAR "SumBufferSize" = "num:i(FrTpGeneral/VendorSpecific/FrTpConnectionBufferSizeUnlimited + count(FrTpGeneral/VendorSpecific/FrTpConnectionBufferSizeUnlimited[. > 0]) + sum(FrTpGeneral/VendorSpecific/FrTpConnectionLimitConfig/*/FrTpConnectionBufferSize) + count(FrTpGeneral/VendorSpecific/FrTpConnectionLimitConfig/*/FrTpConnectionBufferSize[. > 0]))"!]
  [!IF "$SumBufferSize < 256"!]
    [!VAR "ConnectionBufferIdxType" = "'uint8'"!]
  [!ELSEIF "$SumBufferSize < 65536"!]
    [!VAR "ConnectionBufferIdxType" = "'uint16'"!]
  [!ENDIF!]
[!ELSE!]
  [!VAR "FrTpLimitNumberOfConnections" = "'false'"!]
[!ENDIF!]

[!/*
Import GuardedDefine macro
*/!]
[!INCLUDE "CommonMacros.m"!]

[!/*
****************************************************************************************************
* MACRO to get the string for the symbolic name value.
* "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
* "OldName" must hold an unique string for the case that the specification
* conform short name does not exist. This is supplied to be backward compatible.
****************************************************************************************************
*/!]
[!VAR "ShortNameList"="''"!]
[!MACRO "GetSymbolName","ShortNameRef","OldName"!][!/*
*/!][!NOCODE!][!/*
*/!][!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!][!/*
*/!][!IF "$SymbolName = ''"!][!/*
*/!][!VAR "SymbolName" = "concat(name(.),'_',$OldName)"!][!/*
*/!][!ELSE!][!/*
*/!][!VAR "ShortNameList"="concat($ShortNameList,' ',$SymbolName)"!][!/*
*/!][!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!][!/*
*/!][!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$OldName"!] is not unique.[!ENDERROR!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "SymbolName" = "concat($OldName,'_',$SymbolName)"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDNOCODE!][!/*
*/!][!ENDMACRO!]


[!ENDIF!][!// multiple inclusion protection

[!ENDNOCODE!]
