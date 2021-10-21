[!/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!][!//
[!/* *** multiple inclusion protection ***
*/!][!IF "not(var:defined('WDGM_MACROS_M'))"!][!/*
*/!][!VAR "WDGM_MACROS_M"="'true'"!][!/*
/*==================[ macros ]=============================================*/[!/*
/*------------------[ calculation of greatest common divisor ]-------------*/[!/*
*/!][!MACRO "getGCD", "gcd_value1", "gcd_value2"!][!/*
    */!][!IF "$gcd_value2 = 0"!][!/*
      */!][!"$gcd_value1"!][!/*
    */!][!ELSE!][!/*
      */!][!CALL "getGCD", "gcd_value1" = "$gcd_value2", "gcd_value2" = "num:i(num:mod(num:i($gcd_value1),num:i($gcd_value2)))"!][!/*
    */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*
/*------------------[ calculation of least common multiple ]---------------*/[!/*
*/!][!MACRO "getLCM", "lcm_value1", "lcm_value2"!][!/*
      */!][!VAR "gcd"!][!/*
        */!][!CALL "getGCD", "gcd_value1" = "$lcm_value1", "gcd_value2" = "$lcm_value2"!][!/*
      */!][!ENDVAR!][!/*
    */!][!"num:i(num:div(num:mul($lcm_value1,$lcm_value2), $gcd))"!][!/*
*/!][!ENDMACRO!][!/*
/*------------------[ calculation of least common multiple for a mode ]----*/[!/*
*/!][!MACRO "getModeSpecificLCM", "modeRef"!][!/*
    */!][!IF "num:i(count(as:ref($modeRef)/WdgMAliveSupervision/*/WdgMSupervisionReferenceCycle)) >= 1"!][!/*
      */!][!VAR "lcm" = "num:i(as:ref($modeRef)/WdgMAliveSupervision/*[1]/WdgMSupervisionReferenceCycle)"!][!/*
      */!][!LOOP "as:ref($modeRef)/WdgMAliveSupervision/*/WdgMSupervisionReferenceCycle"!][!/*
        */!][!VAR "val1" = "num:i($lcm)"!][!/*
        */!][!VAR "val2" = "num:i(.)"!][!/*
        */!][!VAR "lcm"!][!/*
          */!][!CALL "getLCM", "lcm_value1" = "$val1", "lcm_value2" = "$val2"!][!/*
        */!][!ENDVAR!][!/*
      */!][!ENDLOOP!][!/*
      */!]    [!"$lcm"!][!/*
    */!][!ELSE!][!/*
      */!]    1[!/*
    */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!/*

/*------------------[ calculation of final checkpoints via simple Depth-first search]---------*/[!/*
*/!][!MACRO "getFinalCPsFromInitialCP", "sourceCPRef", "modeId", "extLogSupervIndex", "visited"!][!/*
  */!][!VAR "visited" = "concat($visited, ' ',$sourceCPRef)"!][!/*
  */!][!LOOP "as:modconf('WdgM')[1]/WdgMConfigSet/*[1]/WdgMMode/*[WdgMModeId = $modeId]/WdgMExternalLogicalSupervision/*[@index = $extLogSupervIndex]/WdgMExternalTransition/*[node:value(WdgMExternalTransitionSourceRef) = $sourceCPRef]/WdgMExternalTransitionDestRef"!][!/*
    */!][!VAR "nextCP" = "node:value(.)"!][!/*
    */!][!IF "not(text:contains(text:split($visited), $nextCP))"!][!/*
      */!][!IF "num:i(count(../../../WdgMExternalCheckpointFinalRef/*[node:value(.) = $nextCP])) != 0"!][!/*
        */!][!"$nextCP"!] [!/*
      */!][!ELSE!][!/*
        */!][!CALL "getFinalCPsFromInitialCP", "sourceCPRef" = "$nextCP", "modeId" = "$modeId", "extLogSupervIndex" = "$extLogSupervIndex", "visited" = "$visited"!][!/*
      */!][!ENDIF!][!/*
    */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
*/!][!ENDMACRO!][!/*

**********************************************************************
*/!][!ENDIF!][!/*
*/!][!//
[!ENDNOCODE!][!//