[!/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]
[!IF "not(var:defined('CANSM_MACROS_M'))"!]
[!VAR "CANSM_MACROS_M"="'true'"!]

[!VAR "useTransceiver"="num:i(as:modconf('CanSM')[1]/CanSMGeneral/CanSMMaxNumberOfTransceivers) > 0"!]
[!VAR "useDEM"="as:modconf('CanSM')[1]/ReportToDem/CanSMBusOffReportToDem = 'DEM'"!]

[!/* some resources needs to be available in code if PnSupport or(!!) transceiver are enabled  */!]
[!VAR "globalPnSupport"="as:modconf('CanSM')[1]/CanSMGeneral/CanSMPNSupport"!]
[!VAR "multiCoreSupport"="as:modconf('CanSM')[1]/CanSMGeneral/CanSMMultiCoreSupport"!]

[!/*
*** macros generate line breaks when expanded, so use comment-indentation here ***
*/!][!MACRO "CalcToTicks", "time", "maxTicks"!][!/*
*** results are limited to <= maxTicks, which is only necessary in case of rounding errors, ***
*** because ranges of interacting parameters are appropriately limited ***
   */!][!VAR "macroTicks"="num:i(ceiling($time div as:modconf('CanSM')[1]/CanSMGeneral/CanSMMainFunctionTimePeriod))"!][!/*
   */!][!IF "$macroTicks > num:i($maxTicks)"!][!/*
   */!][!"$maxTicks"!][!/*
   */!][!ELSE!][!/*
   */!][!"$macroTicks"!][!/*
   */!][!ENDIF!][!/*
*/!][!ENDMACRO!][!//

[!ENDIF!]
[!ENDNOCODE!][!//
