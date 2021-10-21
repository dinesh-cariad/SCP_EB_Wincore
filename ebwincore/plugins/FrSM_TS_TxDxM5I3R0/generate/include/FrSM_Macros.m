[!/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('FRSM_MACROS_M'))"!]
[!VAR "FRSM_MACROS_M"="'true'"!]

[!// Converts timeout [s] to main function ticks (as specified in [ecuc_sws_7000])
[!MACRO "TimeoutToTicks", "Time", "MainFunctionCycleTime"!][!/*
*/!][!VAR "TimeoutToTicks_Time_Us" = "$Time * 1000000"!][!/*
*/!][!VAR "TimeoutToTicks_MainFuncCycle_Us" = "$MainFunctionCycleTime * 1000000"!][!/*
*/!][!VAR "TimeoutToTicks_Remainder" = "num:i($TimeoutToTicks_Time_Us mod $TimeoutToTicks_MainFuncCycle_Us)"!][!/*
*/!][!IF "$TimeoutToTicks_Remainder != 0"!][!/*
    */!][!"num:i(floor($TimeoutToTicks_Time_Us div $TimeoutToTicks_MainFuncCycle_Us)+1)"!][!/*
    */!][!WARNING "A timeout is not a multiple of the main function cycle time. The number of ticks for the timeout will be round up."!][!/*
*/!][!ELSE!][!/*
    */!][!"num:i(floor($TimeoutToTicks_Time_Us div $TimeoutToTicks_MainFuncCycle_Us))"!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDMACRO!]

[!ENDIF!]
[!ENDNOCODE!][!//
