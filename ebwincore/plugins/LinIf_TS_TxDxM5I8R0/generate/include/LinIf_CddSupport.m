[!/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('LINIF_CDDSUPPORT_M'))"!]
[!VAR "LINIF_CDDSUPPORT_M"="'true'"!]

[!INCLUDE "../include/LinIf_EOSNotif.m"!]
[!INCLUDE "../include/LinIf_ULCdd.m"!]

[!/*
****************************************************************************************************
* MACRO to enumerate CDD headers
* Result resides in the variable IncludeCDDs
****************************************************************************************************
*/!]
[!MACRO "GetCDDHeaders"!][!NOCODE!]
[!VAR "IncludeCDDs" = "''"!]
[!LOOP "as:modconf('LinIf')/LinIfGeneral/LinIfPublicCddHeaderFile/*"!]
  [!VAR "IncludeCDDs"!][!"$IncludeCDDs"!][!"concat('#include <',node:value(.),'>')"!][!CR!][!ENDVAR!]
[!ENDLOOP!]
[!IF "$ulCddSupported = 'true'"!]
  [!VAR "refdCdds" = "''"!]
  [!VAR "IncludeCDDs"!][!"$IncludeCDDs"!]#include <LinSM_Cbk.h>[!CR!]#include <PduR_LinIf.h>[!CR!][!ENDVAR!]
  [!/* If the referenced Cdds' header does not seem to be included, add it */!]
  [!LOOP "as:modconf('LinIf')/LinIfGlobalConfig/*[1]/LinIfChannel/*/LinIfCddRef"!]
    [!CALL "GetModuleName", "CddConfRef" = "."!]
    [!IF "not(contains($refdCdds, $moduleName))"!]
      [!VAR "refdCdds" = "concat($refdCdds, $moduleName, '|')"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [/* Second loop needed because these are only package names not full paths, can't use node:difference() */]
  [!/* Add the Pdu handling Cdds as well if not listed */!]
  [!LOOP "as:modconf('LinIf')[1]/LinIfGeneral/LinIfCddFunctionsUL/*/CddName"!]
    [!IF "not(contains($refdCdds, .))"!]
      [!VAR "refdCdds" = "concat($refdCdds, ., '|')"!]
    [!ENDIF!]
  [!ENDLOOP!]
  [!LOOP "text:split($refdCdds, '|')"!]
    [!IF "not(text:match($IncludeCDDs, concat(.,'[._]')))"!]
    [!/* If for some reason the header is provided without the _Cbk postfix, omit it */!]
      [!VAR "IncludeCDDs"!][!"$IncludeCDDs"!][!"concat('#include <', .,'_Cbk.h','>')"!][!CR!][!ENDVAR!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDIF!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
****************************************************************************************************
* MACRO to get moduleName
* No proper way of reading apiServicePrefix
* Using the method from asc_Base since at install the modules are transformed to use the
* specified apiServicePrefix as part of package name.
* The result resides in the variable "moduleName".
****************************************************************************************************
*/!]
[!MACRO "GetModuleName","CddConfRef"!]
  [!VAR "apiServicePrefix" = "''"!]
  [!VAR "moduleName"="text:split(as:path(as:dtos(node:ref($CddConfRef))),'/')[last()]"!]
  [!VAR "moduleVariant"="text:split(as:path(as:dtos(node:ref($CddConfRef))),'/')[last()-1]"!]
  [!IF "contains($moduleVariant, '_TS_')"!]
    [!VAR "apiServicePrefix"="substring-before($moduleVariant, '_TS_')"!]
  [!ELSEIF "text:match($moduleVariant,'([^TS].*)_')"!]
    [!VAR "apiServicePrefix"="substring-before($moduleVariant, '_')"!]
  [!ENDIF!]
  [!IF "$apiServicePrefix != ''"!]
    [!VAR "moduleName" = "$apiServicePrefix"!]
  [!ELSE!]
    [!WARNING!][!"$CddConfRef"!]: apiServicePrefix is missing, using [!"$moduleName"!] as fallback[!ENDWARNING!]
  [!ENDIF!]
[!ENDMACRO!]

[!ENDIF!][!// Include guard
[!ENDNOCODE!][!//
