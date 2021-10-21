[!/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//

[!VAR "variantName"="variant:name()"!][!//

[!IF "$variantName = ''"!][!//
[!// ERROR and WARNING evaluate their parameter as an xpath expression thus a dummy concat is used here.
[!VAR "RestartListErr"="concat('Entries in the restart list should be in the same order as in init list zero and one','')"!][!//
[!VAR "RestartListWrn"="concat('There is one entry in the restart list which does not have a counterpart in init list zero or one','')"!][!//
[!ELSE!][!//
[!VAR "RestartListErr"="concat('Entries in the restart list should be in the same order as in init list zero and one for variant ',$variantName)"!][!//
[!VAR "RestartListWrn"="concat('There is one entry in the restart list which does not have a counterpart in init list zero or one for variant ',$variantName)"!][!//
[!ENDIF!][!//

[!NOCODE!][!//
[!//
[!SELECT "EcuMConfiguration/*[1]/EcuMCommonConfiguration"!][!//
[!/* Check for correct order of invocations */!][!//
[!VAR "ModuleServiceList"="''"!][!// Let ModuleServiceList be initially empty
[!LOOP "(EcuMDriverInitListZero/EcuMDriverInitItem/*)"!][!// Loop through init list zero
    [!VAR "ModuleServiceList"="concat($ModuleServiceList,' ',EcuMModuleID,'_', EcuMModuleService)"!][!//
[!ENDLOOP!][!//
[!LOOP "(EcuMDriverInitListOne/EcuMDriverInitItem/*)"!][!// Loop through init list one
  [!IF "EcuMEnableDriver"!][!//
    [!VAR "ModuleServiceList"="concat($ModuleServiceList,' ',EcuMModuleID,'_', EcuMModuleService)"!][!//
  [!ENDIF!][!//
[!ENDLOOP!][!//

[!/* !LINKSTO EcuM2720,1 */!]
[!VAR "RestartModuleService"="''"!][!// Let RestartModuleService be initially empty
[!VAR "PreviousLocation"="-1"!][!// Let PreviousLocation be initially empty
[!VAR "CurrentLocation"="-1"!][!// Let CurrentLocation be initially empty
[!LOOP "EcuMDriverRestartList/EcuMDriverInitItem/*"!][!// Loop through Restart List
  [!IF "EcuMEnableDriver"!][!//
    [!VAR "RestartModuleService"="concat($RestartModuleService,' ',EcuMModuleID,'_', EcuMModuleService)"!][!//
    [!VAR "CurrentLocation"= "text:indexOf( string($ModuleServiceList), string($RestartModuleService) )"!][!//
    [!IF "$CurrentLocation > -1"!][!//
      [!IF "$CurrentLocation <= $PreviousLocation"!][!// if the order is not correct display the error
        [!ERROR "?$RestartListErr"!][!//
      [!ENDIF!][!//
      [!VAR "PreviousLocation" = "$CurrentLocation"!][!// save the current location
    [!ELSE!][!//
      [!WARNING "?$RestartListWrn"!][!//
    [!ENDIF!][!//
    [!VAR "RestartModuleService"="''"!][!// empty the string for future iterations
  [!ENDIF!][!//
[!ENDLOOP!][!//
[!ENDSELECT!][!//
[!ENDNOCODE!][!//
