[!/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2016 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!IF "not(var:defined('ETHIF_CALCTOTICS_M'))"!]
[!VAR "ETHIF_CALCTOTICS_M"="'true'"!]

[!/*
*** macros generate line breaks when expanded, so use comment-indentation here ***
*/!][!MACRO "CalcToTicks", "time", "maxTicks"!][!/*
*** results are limited to <= maxTicks, which is only necessary in case of rounding errors, ***
*** because ranges of interacting parameters are appropriately limited ***
*/!][!IF "node:exists(as:modconf('EthIf')[1]/EthIfGeneral/EthIfMainFunctionStatePeriod)"!][!//
   [!VAR "macroTicks"="num:i(ceiling($time div as:modconf('EthIf')[1]/EthIfGeneral/EthIfMainFunctionStatePeriod))"!][!/*
*/!][!ELSE!][!//
   [!VAR "macroTicks"="num:i(ceiling($time div as:modconf('EthIf')[1]/EthIfGeneral/EthIfMainFunctionPeriod))"!][!/*
*/!][!ENDIF!][!/*
   */!][!IF "$macroTicks > num:i($maxTicks)"!][!/*
   */!][!"num:i($maxTicks)"!][!/*
   */!][!ELSE!][!/*
   */!][!"$macroTicks"!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*
*/!]

[!ENDIF!]