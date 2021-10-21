/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]
[!AUTOSPACING!]

#ifndef FRIF_SYMBOLICNAMES_PBCFG_H
#define FRIF_SYMBOLICNAMES_PBCFG_H

/*
* Include Section
*/

[!INCLUDE "CommonMacros.m"!][!//

/*==================[macros]=================================================*/
[!/* Array for holding Shortnames */!]
[!VAR "ShortNameList"="''"!]

[!/*  MACRO to get the string for the symbolic name value.
   * "ShortNameRef" must reference the container holding the short name. (AUTOSAR short name)
   * "OldName" must hold an unique string for the case that the specification
   * conform short name does not exist. This is supplied to be backward compatible.
*/!][!/*

*/!]
[!MACRO "GetSymbolName","ShortNameRef","OldName"!]
[!VAR "SymbolName" = "asc:getShortName($ShortNameRef)"!]
[!IF "$SymbolName = ''"!]
[!VAR "SymbolName" = "concat(name(../../.),'_',name(.),'_',$OldName)"!]
[!ELSE!]
[!VAR "ShortNameList" = "concat($ShortNameList,' ',$SymbolName)"!]
[!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!]
[!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$OldName"!] is not unique.[!ENDERROR!]
[!ENDIF!]
[!VAR "SymbolName" = "concat($OldName,'_',$SymbolName)"!]
[!ENDIF!]
[!ENDMACRO!]

[!MACRO "GetFrIfPduSymbolName","ObjectRef"!]
[!VAR "SymbolName" = "asc:getShortName($ObjectRef)"!]
[!IF "($SymbolName = 'FrIfPduDirection')"!]
[!VAR "SymbolName" = "name(../.)"!]
[!ELSE!]
[!VAR "ShortNameList" = "concat($ShortNameList,' ',$SymbolName)"!]
[!IF "not(text:uniq(text:split($ShortNameList), $SymbolName))"!]
[!ERROR!] The short name [!"$SymbolName"!] of the schema node [!"$ObjectRef"!] is not unique.[!ENDERROR!]
[!ENDIF!]
[!VAR "SymbolName" = "concat('FrIfTxPdu_',$SymbolName)"!]
[!ENDIF!]
[!ENDMACRO!]
[!/*

*/!][!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfCluster/*"!]
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(../.)"!]
[!CALL "GuardedDefine", "Comment"="'Export symbolic name values for FrIfClstIds'",
  "Name"="concat('FrIfConf_', $SymbolName)"!] [!"FrIfClstIdx"!]U
#if (defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrIf_', @name)"!] [!"FrIfClstIdx"!]U
#endif /* defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!]

[!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfPdu/*/*[@name='FrIfTxPdu']"!]
[!CALL "GetFrIfPduSymbolName", "ObjectRef"="'.'"!]
[!CALL "GuardedDefine", "Comment"="'Export symbolic name values for FrIfTxPduIds'",
  "Name"="concat('FrIfConf_', $SymbolName)"!] [!"FrIfTxPduId"!]U
#if (defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrIf_', ../@name)"!] [!"FrIfTxPduId"!]U
#endif /* defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!/*

*/!][!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*"!]
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(../.)"!]
[!CALL "GuardedDefine", "Comment"="'Export symbolic name values for FrIfCtrlIdxs'",
  "Name"="concat('FrIfConf_', $SymbolName)"!] [!"FrIfCtrlIdx"!]U
#if (defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrIf_', ../../@name, '_', @name)"!] [!"FrIfCtrlIdx"!]U
#endif /* defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!/*

*/!][!LOOP "as:modconf('FrIf')[1]/FrIfConfig/*[1]/FrIfCluster/*/FrIfController/*/FrIfLPdu/*"!]
[!CALL "GetSymbolName", "ShortNameRef"="'.'", "OldName"="name(../.)"!]
[!CALL "GuardedDefine", "Comment"="'Export symbolic name values for FrIfLPduIdxs'",
  "Name"="concat('FrIfConf_', $SymbolName)"!] [!"FrIfLPduIdx"!]U
#if (defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES)
[!CALL "GuardedDefine", "Comment"="'Export symbolic name value with module abbreviation as prefix only (AUTOSAR 4.0 rev2 <= AUTOSAR version)'",
  "Name"="concat('FrIf_', ../../../../@name, '_', ../../@name, '_', @name)"!] [!"FrIfLPduIdx"!]U
#endif /* defined FRIF_PROVIDE_LEGACY_SYMBOLIC_NAMES */
[!ENDLOOP!][!//

#endif /* FRIF_SYMBOLICNAMES_PBCFG_H */

/*==================[end of file]============================================*/
[!ENDCODE!][!//

