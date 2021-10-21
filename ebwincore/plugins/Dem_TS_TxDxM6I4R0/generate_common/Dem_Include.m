[!/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!//
[!/* ======== IMPORTANT NOTE! ===============
   * Dem_Include.m in generate_swcd/swcd_calibration
   * is an svn external of Dem_Include.m in generate/include and
   * This is done due to a include limitation for macro files */!][!//
[!//
[!/* === define maximum space needed ===*/!][!//
[!AUTOSPACING!][!//
[!MACRO "Indent", "Length"!][!//
[!INDENT "0"!][!//
  [!FOR "i" = "1" TO "$Length"!][!//
    [!WS!][!//
  [!ENDFOR!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!SELECT "as:modconf('Dem')[1]"!][!//
[!//
[!/* === original DEM configuration parameters ===*/!][!//
[!VAR "CommonOperationCycleEnabled" = "DemGeneral/DemCommonOperationCycleSupport"!][!//
[!VAR "EventCombinationSupport" = "DemGeneral/DemEventCombinationSupport"!][!//
[!VAR "CalibrationSupport_conf" = "node:exists(DemGeneral/DemCalibrationSupport) and (DemGeneral/DemCalibrationSupport = 'true')"!][!//
[!VAR "CalibrationSupport" = "($CalibrationSupport_conf = 'true') and ($EventCombinationSupport = 'DEM_EVCOMB_DISABLED')"!][!//
[!VAR "CalibrationWithEvCombSupport" = "($CalibrationSupport_conf = 'true') and ($EventCombinationSupport != 'DEM_EVCOMB_DISABLED')"!][!//
[!VAR "DemRteUsage" = "DemGeneral/DemRteUsage"!][!//
[!VAR "NumMultiEventTriggering" = "num:integer(count(DemGeneral/DemMultiEventTriggering/*))"!][!//
[!VAR "DemTaskTime" = "DemGeneral/DemTaskTime"!][!//
[!//
[!/* === derived configuration parameters ===*/!][!//
[!VAR "EventClassSupport" = "$CalibrationSupport = 'true'"!][!//
[!VAR "IndicatorClassSupport" = "($CalibrationSupport = 'true') or ($CalibrationWithEvCombSupport = 'true')"!][!//
[!VAR "AgingAllowedSeperateFlag" = "(DemGeneral/DemAgingAllowedSeperateFlag = 'true') and ($EventClassSupport = 'true')"!][!//
[!//
[!ENDSELECT!][!//
[!/*
 * === Get DemEventClassString ===
 * This Macro will fill the variable EventClassString
 * with a string of the following structure:
 *
 * MultiEventTriggeringClassIdx         T
 * DemIndicatorClass                    I
 * DemAgingCycleCounterThreshold        A
 * DemOBDAgingCycleCounterThreshold     O
 * DemEventFailureCycleCounterThreshold F
 * DemEventPriority                     P
 * CommonOperationCycle                 !
 * DemAgingAllowed                      ?
 * DemMinRatioEvent                     M
 * DemEnableConditionGroupRef           &
 *
 * The String contains the values from the configuration parameter DemEventParameter separated by a
 * meaningful character.
 */!][!//
[!MACRO "GET_EVENT_CLASS_STRING"!][!//
[!INDENT "0"!][!//
  [!VAR "EventClassString" = "''"!][!//
  [!/* calculate multi event triggering index */!][!//
  [!CALL "GET_MULTI_EVENT_TRIGGERING_INDEX", "EVENT_ID" = "../DemEventId"!][!//
  [!IF "$MultiEventTriggeringClassIdx != $NumMultiEventTriggering"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, num:i($MultiEventTriggeringClassIdx), 'T')"!][!//
  [!ELSE!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '-', 'T')"!][!//
  [!ENDIF!][!//
  [!/* calculate indicator class index */!][!//
  [!CALL "GET_INDICATOR_CLASS_INDEX", "EVENT_ID" = "../DemEventId"!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $IndicatorClassIndex, 'I')"!][!//
  [!/* calculate aging allowed and threshold */!][!//
  [!IF "node:exists(DemAgingAllowed)"!][!//
    [!VAR "AgingAllowed" = "DemAgingAllowed"!][!//
  [!ELSE!][!//
    [!VAR "AgingAllowed" = "'false'"!][!//
  [!ENDIF!][!//
  [!IF "$AgingAllowed = 'true'"!][!//
    [!VAR "AgingCycleCounterThreshold" = "num:i(DemAgingCycleCounterThreshold)"!][!//
  [!ELSE!][!//
    [!/* !LINKSTO Dem.AgingAllowedSeperateEntry.Disabled,1 */!][!//
    [!VAR "AgingCycleCounterThreshold" = "num:i(0)"!][!//
  [!ENDIF!][!//
  [!/* calculate aging counter threshold */!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $AgingCycleCounterThreshold, 'A')"!][!//
  [!/* calculate OBD aging counter threshold */!][!//
  [!IF "DemAgingAllowed = 'true' and node:exists(DemOBDAgingCycleCounterThreshold)"!][!//
    [!VAR "OBDAgingCycleCounterThreshold" = "num:i(DemOBDAgingCycleCounterThreshold)"!][!//
  [!ELSE!][!//
    [!VAR "OBDAgingCycleCounterThreshold" = "num:i(0)"!][!//
  [!ENDIF!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $OBDAgingCycleCounterThreshold, 'O')"!][!//
  [!/* calculate OBD failure cycle threshold */!][!//
  [!IF "node:exists(DemEventFailureCycleCounterThreshold)"!][!//
    [!VAR "EventFailureCycleCounterThreshold" = "num:i(DemEventFailureCycleCounterThreshold)"!][!//
  [!ELSE!][!//
    [!VAR "EventFailureCycleCounterThreshold" = "num:i(0)"!][!//
  [!ENDIF!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $EventFailureCycleCounterThreshold, 'F')"!][!//
  [!/* calculate priority */!][!//
  [!IF "node:exists(DemEventPriority)"!][!//
    [!VAR "EventPriority" = "num:i(DemEventPriority)"!][!//
  [!ELSE!][!//
    [!VAR "EventPriority" = "num:i(0)"!][!//
  [!ENDIF!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $EventPriority, 'P')"!][!//
  [!IF "$CommonOperationCycleEnabled = 'true'"!][!//
    [!/* calculate common operation cycle */!][!//
    [!VAR "EventClassString" = "concat($EventClassString, name(as:ref(DemOperationCycleRef)), '!')"!][!//
  [!ENDIF!][!//
  [!/* !LINKSTO Dem.AgingAllowedSeperateEntry.Enabled,1 */!][!//
  [!IF "$AgingAllowedSeperateFlag = 'true'"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, text:toupper($AgingAllowed), '?')"!][!//
  [!ENDIF!][!//
  [!/* calculate minimum ratio */!][!//
  [!IF "node:exists(DemMinRatioEvent) and (DemMinRatioEvent = 'true')"!][!//
    [!VAR "MinRatioEvent" = "num:i(1)"!][!//
  [!ELSE!][!//
    [!VAR "MinRatioEvent" = "num:i(0)"!][!//
  [!ENDIF!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $MinRatioEvent, 'M')"!][!//
  [!/* enable condition group */!][!//
  [!VAR "EnCondGroupRef" = "''"!][!//
  [!IF "node:exists(DemEnableConditionGroupRef)"!][!//
    [!VAR "EnCondGroupRef" = "node:pos(as:ref(DemEnableConditionGroupRef))"!][!//
  [!ELSE!][!//
    [!VAR "EnCondGroupRef" = "'-'"!][!//
  [!ENDIF!][!//
  [!VAR "EventClassString" = "concat($EventClassString, $EnCondGroupRef, '&')"!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!/*
 * === Create DemEventClassList ===
 * This Macro creates a list of unique EventClassString
 * The EventClassString is created with the macro "GET_EVENT_CLASS_STRING"
 * The EventClassString are separate by '#' and ordered first occurrence in a DemEventParameter
 * ordered by DemEventId
 * At the end are added strings with default values for empty configuration entries
 */!][!//
[!MACRO "CREATE_EVENT_CLASS_LIST"!][!//
[!INDENT "0"!][!//
  [!VAR "EventClassList" = "''"!][!//
  [!CALL "CALC_INDICATOR_CLASSES"!][!//
  [!CALL "CALC_MULTI_EVENT_CLASSES"!][!//
  [!LOOP "node:order(/AUTOSAR/TOP-LEVEL-PACKAGES/Dem/ELEMENTS/Dem/DemConfigSet/*/DemEventParameter/*,'DemEventId')"!][!//
    [!VAR "IndicatorClassIndex" = "0"!][!//
    [!SELECT "DemEventClass"!][!//
      [!CALL "GET_EVENT_CLASS_STRING"!][!//
    [!ENDSELECT!][!//
    [!IF "not(contains($EventClassList,$EventClassString))"!][!//
      [!VAR "EventClassList" = "concat($EventClassList, $EventClassString, '#')"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!VAR "NumOfEventClasses" = "num:integer(count(text:split($EventClassList, '#')))"!][!//
  [!VAR "MaxNumberEventClasses" = "DemGeneral/DemMaxNumberEventClasses"!][!//
  [!IF "$NumOfEventClasses > $MaxNumberEventClasses"!][!//
    [!VAR "MaxNumberEventClasses" = "$NumOfEventClasses"!][!//
  [!ENDIF!][!//
  [!/* The EventClassList is filled with strings containing default values for empty
     * configurations entries */!][!//
  [!FOR "EmptyEventClassIdx" = "$NumOfEventClasses"  TO "$MaxNumberEventClasses - 1"!][!//
    [!VAR "EventClassString" = "''"!][!//
    [!/* Default values for Multi-event Triggering ('T'), AgingCycleCounterThreshold ('A'),
       * OBDAgingCycleCounterThreshold ('O'), EventFailureCycleCounterThreshold ('F'),
       * EventPriority ('P') and IndicatorClass ('I') */!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '-', 'T')"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '-', 'I')"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '1', 'A')"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '1', 'O')"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '1', 'F')"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '2', 'P')"!][!//
    [!IF "$CommonOperationCycleEnabled = 'true'"!][!//
      [!/* Default value for CommonOperationCycle */!][!//
      [!VAR "EventClassString" = "concat($EventClassString, name(as:modconf('Dem')[1]/DemGeneral/DemOperationCycle/*[1]), '!')"!][!//
    [!ENDIF!][!//
    [!IF "$AgingAllowedSeperateFlag = 'true'"!][!//
      [!/* Default value for AgingAllowed */!][!//
      [!VAR "EventClassString" = "concat($EventClassString, 'TRUE', '?')"!][!//
    [!ENDIF!][!//
    [!/* Default values for MinRatioEvent and EnableConditionGroupRef */!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '0', 'M')"!][!//
    [!VAR "EventClassString" = "concat($EventClassString, '-', '&')"!][!//
    [!VAR "EventClassList" = "concat($EventClassList, $EventClassString, '#')"!][!//
  [!ENDFOR!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!/*
 * === Get SizeOfFreezeFrameClass ===
 * The Macro shall be run in contex of a DemFreezeFrameClass and it computes:
 * - the total size of all DidData assigned to this FreezeFrame and assignes
 * it to SizeOfFreezeFrameClass
 * - the number of Dids assigned to the FreezeFrame and assigns it to
 * SizeOfFreezeFrameClassNumDid.
 */!][!//
[!MACRO "GET_SIZE_OF_FREEEZE_FRAME_CLASS"!][!//
  [!VAR "SizeOfFreezeFrameClass" = "0"!][!//
  [!VAR "SizeOfFreezeFrameClassNumDid" = "0"!][!//
  [!LOOP "./DemDidClassRef/*"!][!//
    [!LOOP "node:ref(.)/DemDidDataElementClassRef/*"!][!//
      [!VAR "SizeOfFreezeFrameClass" = "$SizeOfFreezeFrameClass + as:ref(.)/DemDataElementDataSize"!][!//
    [!ENDLOOP!][!//
    [!VAR "SizeOfFreezeFrameClassNumDid" = "$SizeOfFreezeFrameClassNumDid + 1"!][!//
  [!ENDLOOP!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Get NumFFRecords ===
 * This Macro shall run in context of a DemEventParameter.
 * This Macro outputs NumFFRecords in whitch is stored the number of freeze
 * frame records.
 */!][!//
[!MACRO "GET_NUM_FF_RECORDS"!][!//
  [!IF "../../../../DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CALCULATED'"!][!//
    [!VAR "NumFFRecords" = "DemMaxNumberFreezeFrameRecords"!][!//
  [!ELSE!][!//
    [!VAR "NumFFRecords" = "count(as:ref(DemFreezeFrameRecNumClassRef)/DemFreezeFrameRecordNumber/*)"!][!//
  [!ENDIF!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Is internal data element referenced ===
 * This Macro outputs IsIntDataReferenced which indicates whether selected internal data element is used.
 */!][!//
[!MACRO "IS_INT_DATA_REFERENCED", "InternalDataElement"!][!//
[!VAR "IsIntDataReferenced" = "text:contains(node:refs(node:refs(node:refs(DemConfigSet/*/DemEventParameter/*/DemExtendedDataClassRef)/DemExtendedDataRecordClassRef/*)/DemDataElementClassRef/*)/DemInternalDataElement, $InternalDataElement)"!]
[!ENDMACRO!][!//
[!//
[!/*
 * === Get VccOcc5Used ===
 * This Macro outputs VccOcc5Used which indicates whether OCC5 is used.
 */!][!//
[!MACRO "GET_VCC_OCC5_USED"!][!//
[!INDENT "0"!][!//
  [!VAR "VccOcc5Used" = "0"!][!//
     [!/* !LINKSTO dsn.Dem.OBD.OCC5.DemInternalDataElement,1,VCC_DEM_006_Req171v1_Implicit1,1 */!][!//
     [!CALL "IS_INT_DATA_REFERENCED", "InternalDataElement" = "'DEM_OCC5'"!][!//
     [!IF "$IsIntDataReferenced"!][!//
       [!VAR "VccOcc5Used" = "1"!][!//
     [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === GET_WIDTH ===
 * returns the required bits for a variable
 */!][!//
[!MACRO "GET_WIDTH", "PNumber", "context"!][!//
[!INDENT "0"!][!//
  [!IF "$PNumber < 0"!][!//
    [!VAR "NumberOfBits" = "0"!][!//
  [!ELSEIF "$PNumber < 2"!][!//
    [!VAR "NumberOfBits" = "1"!][!//
  [!ELSEIF "$PNumber < 4"!][!//
    [!VAR "NumberOfBits" = "2"!][!//
  [!ELSEIF "$PNumber < 8"!][!//
    [!VAR "NumberOfBits" = "3"!][!//
  [!ELSEIF "$PNumber < 16"!][!//
    [!VAR "NumberOfBits" = "4"!][!//
  [!ELSEIF "$PNumber < 32"!][!//
    [!VAR "NumberOfBits" = "5"!][!//
  [!ELSEIF "$PNumber < 64"!][!//
    [!VAR "NumberOfBits" = "6"!][!//
  [!ELSEIF "$PNumber < 128"!][!//
    [!VAR "NumberOfBits" = "7"!][!//
  [!ELSEIF "$PNumber < 256"!][!//
    [!VAR "NumberOfBits" = "8"!][!//
  [!ELSEIF "$PNumber < 512"!][!//
    [!VAR "NumberOfBits" = "9"!][!//
  [!ELSEIF "$PNumber < 1024"!][!//
    [!VAR "NumberOfBits" = "10"!][!//
  [!ELSEIF "$PNumber < 2048"!][!//
    [!VAR "NumberOfBits" = "11"!][!//
  [!ELSEIF "$PNumber < 4096"!][!//
    [!VAR "NumberOfBits" = "12"!][!//
  [!ELSEIF "$PNumber < 8192"!][!//
    [!VAR "NumberOfBits" = "13"!][!//
  [!ELSEIF "$PNumber < 16384"!][!//
    [!VAR "NumberOfBits" = "14"!][!//
   [!ELSE!][!//
    [!ERROR!]Unexpected condition in Dem 'GET_WIDTH' macro. Given number is too big! Context: [!"$context"!] Number:[!"$PNumber"!][!ENDERROR!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!/*
 * === LOG ===
 * returns the logarithm to base of 2
 */!][!//
[!MACRO "LOG", "PNumber", "context"!][!//
  [!CALL "GET_WIDTH", "PNumber" = "$PNumber", "context" = "$context"!][!//
  [!"num:i($NumberOfBits)"!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Indicators macros ===
 */!][!//
[!//
[!/*
 * === Indicator Classes ===
 * Returns the indicator class index of given EventId
 */!][!//
[!MACRO "GET_INDICATOR_CLASS_INDEX", "EVENT_ID"!][!//
[!INDENT "0"!][!//
  [!LOOP "text:split($EventIndicatorClassIndexList, '#')"!][!//
    [!IF "contains(., concat('!', num:i($EVENT_ID), ':'))"!][!//
      [!VAR "IndicatorClassIndex" = "text:split(., ':')[position() = 2]"!][!//
      [!IF "$IndicatorClassIndex != '-'"!][!//
        [!VAR "IndicatorClassIndex" = "num:i($IndicatorClassIndex)"!][!//
      [!ENDIF!][!//
      [!BREAK!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Number of Indicator Classes ===
 * Returns the number of indicator classes
 */!][!//
[!MACRO "GET_NUMBER_OF_INDICATOR_CLASSES"!][!//
[!INDENT "0"!][!//
  [!VAR "NumberOfIndicatorClasses" = "count(text:split($IndicatorClassList, '#'))"!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Indicator Classes ===
 * Calculates all indicator classes
 *
 * Classes are separated by #
 * Class entries (DemIndicatorAttributes) are separated by +
 */!][!//
[!/* !LINKSTO Dem.IndicatorClass.Combination,1 */!][!//
[!MACRO "CALC_INDICATOR_CLASSES"!][!//
[!INDENT "0"!][!//
  [!VAR "ClassIndex" = "0"!][!//
  [!VAR "IndicatorClassList" = "''"!][!//
  [!VAR "EventIndicatorClassIndexList" = "''"!][!//
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!][!//
    [!VAR "IndicatorClass" = "''"!][!//
    [!IF "count(./DemEventClass/DemIndicatorAttribute/*) > 0"!][!//
      [!LOOP "node:order(./DemEventClass/DemIndicatorAttribute/*, 'name(as:ref(DemIndicatorRef))')"!][!//
        [!IF "./DemIndicatorFailureCycleSource = 'DEM_FAILURE_CYCLE_INDICATOR'"!][!//
          [!VAR "FailureThreshold" = "node:value(./DemIndicatorFailureCycleCounterThreshold)"!][!//
          [!VAR "IsIndFailureCycleSrcEvent" = "'FALSE'"!][!//
        [!ELSE!][!//
          [!/* check if event is combined */!][!//
          [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
            [!VAR "FailureThreshold" = "node:value(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
          [!ELSE!][!//
            [!VAR "FailureThreshold" = "node:value(../../DemEventFailureCycleCounterThreshold)"!][!//
          [!ENDIF!][!//
          [!VAR "IsIndFailureCycleSrcEvent" = "'TRUE'"!][!//
        [!ENDIF!][!//
        [!IF "$CommonOperationCycleEnabled = 'false'"!][!//
          [!IF "./DemIndicatorFailureCycleSource = 'DEM_FAILURE_CYCLE_INDICATOR'"!][!//
            [!VAR "FailureCycle" = "node:name(as:ref(./DemIndicatorFailureCycleRef))"!][!//
          [!ELSE!][!//
            [!/* check if event is combined */!][!//
            [!IF "node:exists(../../../DemEvtCmbCommonParamMaster)"!][!//
              [!VAR "FailureCycle" = "node:name(as:ref(../../../../*[(DemDTCClassRef=node:current()/../../../DemDTCClassRef) and (DemEvtCmbCommonParamMaster = 'true')]/DemEventClass/DemEventFailureCycleRef))"!][!//
            [!ELSE!][!//
              [!VAR "FailureCycle" = "node:name(as:ref(../../DemEventFailureCycleRef))"!][!//
            [!ENDIF!][!//
          [!ENDIF!][!//
          [!VAR "HealingCycle" = "node:name(as:ref(DemIndicatorHealingCycleRef))"!][!//
          [!VAR "IndicatorClass" = "concat($IndicatorClass, ' ', name(as:ref(DemIndicatorRef)), ' ', DemIndicatorBehaviour, ' ', num:i($FailureThreshold), ' ', num:i(DemIndicatorHealingCycleCounterThreshold), ' ',$IsIndFailureCycleSrcEvent, ' ', $FailureCycle, ' ', $HealingCycle, '+')"!][!//
        [!ELSE!][!//
          [!/* Common operation cycle*/!][!//
          [!VAR "IndicatorClass" = "concat($IndicatorClass, ' ', name(as:ref(DemIndicatorRef)), ' ', DemIndicatorBehaviour, ' ', num:i($FailureThreshold), ' ', num:i(DemIndicatorHealingCycleCounterThreshold), ' ',$IsIndFailureCycleSrcEvent, '+')"!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
      [!VAR "Index" = "0"!][!//
      [!IF "not(contains($IndicatorClassList, concat($IndicatorClass, '#')))"!][!//
        [!VAR "IndicatorClassList" = "concat($IndicatorClassList, $IndicatorClass, '#')"!][!//
        [!VAR "Index" = "num:i($ClassIndex)"!][!//
        [!VAR "ClassIndex" = "$ClassIndex + 1"!][!//
      [!ELSE!][!//
        [!VAR "IndexFound" = "'false'"!][!//
        [!LOOP "text:split($IndicatorClassList, '#')"!][!//
          [!IF ". != $IndicatorClass"!][!//
            [!VAR "Index" = "num:i($Index + 1)"!][!//
          [!ELSE!][!//
            [!VAR "IndexFound" = "'true'"!][!//
            [!BREAK!][!//
          [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!IF "$IndexFound = 'false'"!][!//
          [!VAR "Index" = "'-'"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!VAR "Index" = "'-'"!][!//
    [!ENDIF!][!//
    [!VAR "EventIndicatorClassIndexList" = "concat($EventIndicatorClassIndexList, '!', DemEventId, ':', $Index, '#')"!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Number of Indicator Links ===
 * Returns the (maximum) number of indicator links
 */!][!//
[!MACRO "GET_NUMBER_OF_INDICATOR_LINKS"!][!//
[!INDENT "0"!][!//
  [!IF "$IndicatorClassSupport = 'true'"!][!//
    [!CALL "CALC_INDICATOR_CLASSES"!][!//
    [!CALL "GET_NUMBER_OF_INDICATOR_CLASSES"!][!//
    [!IF "$NumberOfIndicatorClasses = 0"!][!//
      [!/* If no indicator is configured in the indicator list, then DemMaxNumberIndicatorClasses will be taken into consideration
         * when calibration support is enabled. DemMaxNumberIndicatorClasses specifies how many instances of the class
         * DemIndicatorClass that should be allocated at least */!][!//
      [!VAR "NumIndicatorLinks" = "DemGeneral/DemMaxNumberIndicatorClasses"!][!//
    [!ELSE!][!//
      [!/* Subtracting of one must be done because of the class separator # at the end */!][!//
      [!VAR "NumIndicatorLinks" = "count(text:split($IndicatorClassList, '+')) - 1"!][!//
      [!IF "($NumIndicatorLinks != 0) and ($NumberOfIndicatorClasses < DemGeneral/DemMaxNumberIndicatorClasses)"!][!//
        [!/* Add additional requested classes (DemGeneral/DemMaxNumberIndicatorClasses) */!][!//
        [!VAR "NumIndicatorLinks" = "$NumIndicatorLinks + DemGeneral/DemMaxNumberIndicatorClasses - $NumberOfIndicatorClasses"!][!//
      [!ENDIF!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!VAR "NumIndicatorLinks" = "count(DemConfigSet/*/DemEventParameter/*/DemEventClass/DemIndicatorAttribute/*)"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
 * === Packed EventDesc macros ===
 */!][!//
[!/* === Packed EventDesc macro for UDSDTCValue === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_UDS_DTC_VALUE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemUdsDTC)"!][!//
    [!WS "$Indent"!]/* UDS-DTC value */
    [!WS "$Indent"!][!"$Prefix"!][!"translate(num:inttohex(as:ref(DemDTCClassRef)/DemUdsDTC, 6), 'abcdef', 'ABCDEF')"!]U
  [!ELSE!][!/* No DTC class or no UDS-DTC configured */!][!//
    [!WS "$Indent"!]/* No UDS-DTC value */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_NO_DTC) )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for OBDDTCValue === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_OBD_DTC_VALUE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemObdDTC)"!][!//
    [!WS "$Indent"!][!"$Prefix"!][!"translate(num:inttohex(as:ref(DemDTCClassRef)/DemObdDTC, 4), 'abcdef', 'ABCDEF')"!]U, /* OBD-DTC value */
  [!ELSE!][!/* No DTC class or no OBD-DTC configured */!][!//
    [!WS "$Indent"!][!"$Prefix"!](uint16)(DEM_NO_DTC), /* No OBD-DTC value */
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for MinRatio value === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_OBD_MIN_RATIO_VALUE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "node:exists(DemEventClass/DemMinRatioEvent) and DemEventClass/DemMinRatioEvent = 'true'"!][!//
    [!WS "$Indent"!][!"$Prefix"!]1U /* Minimum Ratio Event is set */
  [!ELSE!][!//
    [!WS "$Indent"!][!"$Prefix"!]0U /* Minimum Ratio Event is not set */
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for OBD Readiness value === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_OBD_READINESS_VALUE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "$CalibrationSupport = 'false'"!][!//
    [!WS "$Indent"!][!"$Prefix"!]((uint16)(DEM_OBD_READINESS_MASK & [!"DemEventClass/DemEventOBDReadinessGroup"!]) <<[!//
    [!WS!]DEM_OBD_READINESS_OFFSET) /* OBD Readiness Group is set */
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for DTCOrigin === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_DTC_ORIGIN", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!WS "$Indent"!]/* Origin of DTC */
  [!WS "$Indent"!][!"$Prefix"!]( (uint32)[!//
  [!/* exactly one event destination can be selected, ensured in xdm */!][!//
  [!IF "DemEventClass/DemEventDestination = 'DEM_DTC_ORIGIN_PRIMARY_MEMORY'"!][!//
    [!WS!](DEM_DTCORIGIN_MASK & (DEM_DTC_ORIGIN_PRIMARY_MEMORY - 1U))[!//
  [!ENDIF!][!//
  [!IF "DemEventClass/DemEventDestination = 'DEM_DTC_ORIGIN_SECONDARY_MEMORY'"!][!//
    [!WS!](DEM_DTCORIGIN_MASK & (DEM_DTC_ORIGIN_SECONDARY_MEMORY - 1U))[!//
  [!ENDIF!][!//
  [!IF "DemEventClass/DemEventDestination = 'DEM_DTC_ORIGIN_MIRROR_MEMORY'"!][!//
    [!WS!](DEM_DTCORIGIN_MASK & (DEM_DTC_ORIGIN_MIRROR_MEMORY - 1U))[!//
  [!ENDIF!][!//
  [!WS!]<< DEM_DTCORIGIN_OFFSET )
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for DTCSeverity === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_DTC_SEVERITY", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemDTCSeverity)"!][!//
    [!IF "as:ref(DemDTCClassRef)/DemDTCSeverity = 'DEM_DTC_SEV_NO_SEVERITY'"!][!//
      [!WS "$Indent"!]/* Severity of DTC: corresponds to DEM_SEVERITY_NO_SEVERITY */
      [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCSEVERITY_MASK & 0U) << DEM_DTCSEVERITY_OFFSET )
    [!ELSEIF "as:ref(DemDTCClassRef)/DemDTCSeverity = 'DEM_DTC_SEV_MAINTENANCE_ONLY'"!][!//
      [!WS "$Indent"!]/* Severity of DTC: corresponds to DEM_SEVERITY_MAINTENANCE_ONLY */
      [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCSEVERITY_MASK & 1U) << DEM_DTCSEVERITY_OFFSET )
    [!ELSEIF "as:ref(DemDTCClassRef)/DemDTCSeverity = 'DEM_DTC_SEV_CHECK_AT_NEXT_HALT'"!][!//
      [!WS "$Indent"!]/* Severity of DTC: corresponds to DEM_SEVERITY_CHECK_AT_NEXT_HALT */
      [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCSEVERITY_MASK & 2U) << DEM_DTCSEVERITY_OFFSET )
    [!ELSEIF "as:ref(DemDTCClassRef)/DemDTCSeverity = 'DEM_DTC_SEV_IMMEDIATELY'"!][!//
      [!WS "$Indent"!]/* Severity of DTC: corresponds to DEM_SEVERITY_CHECK_IMMEDIATELY */
      [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCSEVERITY_MASK & 3U) << DEM_DTCSEVERITY_OFFSET )
[!/* ELSE-case will never appear, ensured by enumeration in xdm */!][!//
    [!ENDIF!][!//
  [!ELSE!][!//
    [!WS "$Indent"!]/* No severity specified for DTC */
    [!/* !LINKSTO Dem.ASR431.SWS_Dem_01292,1 */!][!//
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCSEVERITY_MASK & 0U) << DEM_DTCSEVERITY_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EnablePrestorage === */!][!//
[!VAR "MaxNumPrestoredFF" = "num:integer(count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemFFPrestorageSupported = 'true']))"!]
[!MACRO "EVENT_DESC_BITFIELD_ENABLE_PRESTORAGE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
[!IF "$MaxNumPrestoredFF = 0"!][!//
  [!WS "$Indent"!]/* Prestorage disabled */
[!ELSE!][!//
  [!IF "DemEventClass/DemFFPrestorageSupported = 'true'"!][!//
    [!WS "$Indent"!]/* Prestorage enabled */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_ENABLEPRESTORAGE_MASK & 1U) << DEM_ENABLEPRESTORAGE_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* Prestorage disabled */
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EventKind === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_EVENT_KIND", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!WS "$Indent"!]/* Event kind is [!"substring-after(DemEventKind, 'DEM_EVENT_KIND_')"!] */
  [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTKIND_MASK & [!"DemEventKind"!]) <<[!//
  [!WS!]DEM_EVENTKIND_OFFSET )
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for IndicatorUsed === */!][!//
[!CALL "GET_NUMBER_OF_INDICATOR_LINKS"!][!//
[!MACRO "EVENT_DESC_BITFIELD_INDICATOR_USED", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
[!IF "$NumIndicatorLinks = 0"!][!//
  [!WS "$Indent"!]/* No warning indicator used */
[!ELSE!][!//
  [!IF "count(DemEventClass/DemIndicatorAttribute/*) != 0"!][!//
    [!WS "$Indent"!]/* Warning indicator used */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_INDICATORUSED_MASK & 1U) << DEM_INDICATORUSED_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* No warning indicator used */
  [!ENDIF!][!//
[!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for ImmediateStorage === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_IMMEDIATE_STORAGE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "(node:exists(DemDTCClassRef) and as:ref(DemDTCClassRef)/DemImmediateNvStorage = 'true') or (../../../../DemGeneral/DemImmediateStorageInternalEvents = 'true' and not(node:exists(DemDTCClassRef)))"!][!//
    [!WS "$Indent"!]/* Immediate storage enabled */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_IMMEDIATESTORAGE_MASK & 1U) <<[!//
    [!WS!]DEM_IMMEDIATESTORAGE_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* Immediate storage disabled */
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EventSignificance === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_EVENT_SIGNIFICANCE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!WS "$Indent"!]/* DTC Significance */
  [!IF " node:exists(DemEventClass/DemEventSignificance) and DemEventClass/DemEventSignificance = 'DEM_EVENT_SIGNIFICANCE_OCCURRENCE'"!][!//
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTSIGNIFICANCE_MASK & DEM_EVENT_SIGNIFICANCE_OCCURRENCE) <<[!//
    [!WS!]DEM_EVENTSIGNIFICANCE_OFFSET )
  [!ELSEIF "node:exists(DemEventClass/DemEventSignificance) and (DemEventClass/DemEventSignificance = 'DEM_EVENT_SIGNIFICANCE_FAULT')"!][!//
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTSIGNIFICANCE_MASK & DEM_EVENT_SIGNIFICANCE_FAULT) <<[!//
    [!WS!]DEM_EVENTSIGNIFICANCE_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTSIGNIFICANCE_MASK & DEM_EVENT_SIGNIFICANCE_FAULT) <<[!//
    [!WS!]DEM_EVENTSIGNIFICANCE_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for FreezeFrames & FFRecInfo === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_FREEZE_FRAMES", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "node:exists(DemFreezeFrameClassRef)"!][!//
    [!WS "$Indent"!]/* Freeze frame class */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_FFCLASSIDX_MASK &[!//
    [!WS!]DEM_FFCLS_IDX_[!"name(as:ref(DemFreezeFrameClassRef))"!]) << DEM_FFCLASSIDX_OFFSET)
    [!IF "../../../../DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CALCULATED'"!][!//
      [!WS "$Indent"!]/* Maximal number of freeze frames */
      [!WS "$Indent"!]| ( (uint32)(DEM_FFRECINFO_MASK & [!"DemMaxNumberFreezeFrameRecords"!]U) << DEM_FFRECINFO_OFFSET )
    [!ELSE!][!//
      [!WS "$Indent"!]/* Index of freeze frame record numeration class */
      [!WS "$Indent"!]| ( (uint32)(DEM_FFRECINFO_MASK & DEM_FFRECNUMCLS_IDX_[!"name(as:ref(DemFreezeFrameRecNumClassRef))"!]) <<[!//
      [!WS!]DEM_FFRECINFO_OFFSET )
    [!ENDIF!][!//
  [!ELSEIF "node:exists(../../../../DemGeneral/DemCommonFFDataClassRef)"!][!//
    [!WS "$Indent"!]/* Freeze frame class */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_FFCLASSIDX_MASK & DEM_FFCLS_NULL_IDX) << DEM_FFCLASSIDX_OFFSET )
    [!IF "../../../../DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CALCULATED'"!][!//
      [!WS "$Indent"!]/* Maximal number of freeze frames */
      [!WS "$Indent"!]| ( (uint32)(DEM_FFRECINFO_MASK & [!"DemMaxNumberFreezeFrameRecords"!]U) << DEM_FFRECINFO_OFFSET )
    [!ELSE!][!//
      [!WS "$Indent"!]/* Index of freeze frame record numeration class */
      [!WS "$Indent"!]| ( (uint32)(DEM_FFRECINFO_MASK & DEM_FFRECNUMCLS_IDX_[!"name(as:ref(DemFreezeFrameRecNumClassRef))"!]) <<[!//
      [!WS!]DEM_FFRECINFO_OFFSET )
    [!ENDIF!][!//
  [!ELSE!][!//
    [!WS "$Indent"!]/* No freeze frame class */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_FFCLASSIDX_MASK & DEM_FFCLS_NULL_IDX) << DEM_FFCLASSIDX_OFFSET )
    [!WS "$Indent"!]/* No freeze frame records */
    [!IF "../../../../DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CONFIGURED' and count(../../../../DemGeneral/DemFreezeFrameRecNumClass/*) > 0"!][!//
      [!WS "$Indent"!]| ( (uint32)(DEM_FFRECINFO_MASK & DEM_FFRECNUMCLS_NULL) << DEM_FFRECINFO_OFFSET )
    [!ELSE!][!//
      [!WS "$Indent"!]| ( (uint32)(DEM_FFRECINFO_MASK & 0U) << DEM_FFRECINFO_OFFSET )
    [!ENDIF!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EDClassIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_EXTENDED_DATA_CLASS", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
[!/*  - [EDClassIdx] - */!][!//
  [!IF "node:exists(DemExtendedDataClassRef)"!][!//
    [!WS "$Indent"!]/* Extended data class */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EDCLASSIDX_MASK & DEM_EDCLS_IDX_[!"name(as:ref(DemExtendedDataClassRef))"!]) <<[!//
    [!WS!]DEM_EDCLASSIDX_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* No extended data class */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EDCLASSIDX_MASK & DEM_EDCLS_NULL_IDX) << DEM_EDCLASSIDX_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for OpCycleIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_OPERATION_CYCLE", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!WS "$Indent"!]/* Operation cycle Id */
  [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_OPCYCLEIDX_MASK & [!"name(as:ref(DemEventClass/DemOperationCycleRef))"!]) <<[!//
  [!WS!]DEM_OPCYCLEIDX_OFFSET )
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for DTCConfigIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_DTC_CONFIG_IDX", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!VAR "DemDTCClassRefName" = "0"!][!//
  [!VAR "DemDTCCounter" = "0"!][!//
  [!IF "node:exists(DemDTCClassRef)"!][!//
    [!VAR "DemDTCClassRefName" = "name(as:ref(DemDTCClassRef))"!][!//
    [!LOOP "text:split($DemDTCClassList, '#')"!][!//
      [!IF "$DemDTCClassRefName = ."!][!//
        [!WS "$Indent"!]/* Index of DTC Class */
        [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCCONFIDX_MASK & [!"num:i($DemDTCCounter)"!]U) << DEM_DTCCONFIDX_OFFSET )
        [!BREAK!][!//
      [!ENDIF!][!//
      [!VAR "DemDTCCounter" = "$DemDTCCounter + 1"!][!//
    [!ENDLOOP!][!//
  [!ELSE!][!//
    [!VAR "DemDTCClassRefName" = "node:name(.)"!][!//
    [!LOOP "text:split($DemDTCClassList, '#')"!][!//
      [!IF "$DemDTCClassRefName = ."!][!//
        [!WS "$Indent"!]/* Index of dummy DTC Class */
        [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCCONFIDX_MASK &[!"num:i($DemDTCCounter)"!]U) << DEM_DTCCONFIDX_OFFSET )
        [!BREAK!][!//
      [!ENDIF!][!//
      [!VAR "DemDTCCounter" = "$DemDTCCounter + 1"!][!//
    [!ENDLOOP!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for AgingCounterThreshold === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_AGING_CTR_THRESHOLD", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "DemEventClass/DemAgingAllowed = 'true'"!][!//
    [!WS "$Indent"!]/* Limit of DTC aging cycles */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_AGINGCTRTHRESHOLD_MASK & [!"DemEventClass/DemAgingCycleCounterThreshold"!]U) <<[!//
    [!WS!]DEM_AGINGCTRTHRESHOLD_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* Aging disabled (AgingCycles = 0) */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)0U << DEM_AGINGCTRTHRESHOLD_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for DTCGroup === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_DTC_GROUP", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
[!/*  find group to which the configured DTC belongs to */!][!//
  [!IF "node:exists(DemDTCClassRef) and node:exists(as:ref(DemDTCClassRef)/DemUdsDTC)"!][!//
    [!VAR "GroupName" = "'ALL_DTC'"!][!//
    [!IF "(node:exists(../../../../DemGeneral/DemGroupOfDTC/*[(name(.) != 'DEM_DTC_GROUP_EMISSION_REL_DTCS') and (DemGroupDTCs < as:ref(node:current()/DemDTCClassRef)/DemUdsDTC)]))"!][!//
      [!VAR "GroupName" = "substring(name(node:order(../../../../DemGeneral/DemGroupOfDTC/*[(DemGroupDTCs < as:ref(node:current()/DemDTCClassRef)/DemUdsDTC)], './DemGroupDTCs')[last()]), 15)"!][!//
    [!ENDIF!][!//
    [!WS "$Indent"!]/* Group of DTC */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCGROUP_MASK & DEM_DTCGRP_IDX_[!"$GroupName"!]) << DEM_DTCGROUP_OFFSET )
  [!ELSE!][!/* No UDS DTC configured */!][!//
    [!WS "$Indent"!]/* No group of DTC */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCGROUP_MASK & DEM_DTCGRP_IDX_NO_DTC) << DEM_DTCGROUP_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EventClass === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_EVENT_CLASS", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
[!/*  - Create current event class string - */!][!//
  [!VAR "EventClassIdx" = "0"!][!//
  [!SELECT "DemEventClass"!][!//
    [!CALL "GET_EVENT_CLASS_STRING"!][!//
  [!ENDSELECT!][!//
  [!LOOP "text:split($EventClassList, '#')"!][!//
    [!IF ". = $EventClassString"!][!//
      [!BREAK!][!//
    [!ELSE!][!//
      [!VAR "EventClassIdx" = "$EventClassIdx + 1"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
  [!WS "$Indent"!]/* Event Class Description Index */
  [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTCLASSDESCIDX_MASK & [!"num:integer($EventClassIdx)"!]U) <<[!//
  [!WS!]DEM_EVENTCLASSDESCIDX_OFFSET )
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EnCondGrpIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_ENABLE_CONDITION_GROUP_IDX", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!VAR "EnCondGrpCtrIdx" = "0"!][!//
  [!IF "node:exists(DemEventClass/DemEnableConditionGroupRef)"!][!//
    [!LOOP "DemEventClass/DemEnableConditionGroupRef"!][!//
      [!VAR "EnCondGrpCtrIdx" = "node:pos(as:ref(.))"!][!//
    [!ENDLOOP!][!//
    [!WS "$Indent"!]/* Index of enable condition group */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_ENCONDGRPIDX_MASK & [!"substring(concat(string(num:integer($EnCondGrpCtrIdx)), 'U '), 1, 6)"!]) <<[!//
    [!WS!]DEM_ENCONDGRPIDX_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* No enable condition group referenced */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_ENCONDGRPIDX_MASK & DEM_ENCONDITIONGROUP_UNUSED) <<[!//
    [!WS!]DEM_ENCONDGRPIDX_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for MultiEventTriggeringClassIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_MULTI_EVENT_TRIGGERING_IDX", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!CALL "GET_MULTI_EVENT_TRIGGERING_INDEX", "EVENT_ID" = "./DemEventId"!][!//
  [!IF "$MultiEventTriggeringClassIdx != $NumMultiEventTriggering"!][!//
    [!WS "$Indent"!]/* Index of multi event triggering configuration */
    [!WS "$Indent"!][!"$Prefix"!]( (uint16)(DEM_MULTIEVENTTRIGGERINGIDX_MASK & [!"num:i($MultiEventTriggeringClassIdx)"!]U) <<[!//
    [!WS!]DEM_MULTIEVENTTRIGGERINGIDX_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* No multi event triggering configuration referenced */
    [!WS "$Indent"!][!"$Prefix"!]( (uint16)(DEM_MULTIEVENTTRIGGERINGIDX_MASK & DEM_MULTIEVENTTRIGGERINGIDX_UNUSED) <<[!//
    [!WS!]DEM_MULTIEVENTTRIGGERINGIDX_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EventFailureClassIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_EVENT_FAILUIRE_CLASS_IDX", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!VAR "EvFailureFound" = "0"!][!//
[!/* check if event is combined */!][!//
  [!IF "node:exists(DemEvtCmbCommonParamMaster)"!][!//
    [!SELECT "as:modconf('Dem')[1]/DemConfigSet/*[1]/DemEventParameter/*[DemDTCClassRef = node:current()/DemDTCClassRef and DemEvtCmbCommonParamMaster = 'true']/DemEventClass"!][!//
      [!IF "node:exists(./DemEventFailureCycleCounterThreshold)"!][!//
        [!VAR "EventFailureCycle" = "node:name(as:ref(./DemEventFailureCycleRef))"!][!//
        [!VAR "EventFailureThreshold" = "node:value(./DemEventFailureCycleCounterThreshold)"!][!//
        [!VAR "EvFailureFound" = "1"!][!//
      [!ENDIF!][!//
    [!ENDSELECT!][!//
  [!ELSE!][!//
    [!IF "node:exists(DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
      [!IF "$CommonOperationCycleEnabled = 'true'"!][!//
        [!VAR "EventFailureCycle" = "node:name(as:ref(DemEventClass/DemOperationCycleRef))"!][!//
      [!ELSE!][!//
        [!VAR "EventFailureCycle" = "node:name(as:ref(DemEventClass/DemEventFailureCycleRef))"!][!//
      [!ENDIF!][!//
      [!VAR "EventFailureThreshold" = "node:value(DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
      [!VAR "EvFailureFound" = "1"!][!//
    [!ENDIF!][!//
  [!ENDIF!][!//
  [!IF "$EvFailureFound = 1"!][!//
    [!VAR "FailureClassIdx" = "0"!][!//
[!/* search list of failure cycle configurations for existing combination */!][!//
    [!LOOP "text:split($FailureCycleCfgList, '#')"!][!//
      [!IF ". = concat($EventFailureCycle, '*', $EventFailureThreshold)"!][!//
[!/* failure cycle configuration item found, stop searching */!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
      [!VAR "FailureClassIdx" = "num:i($FailureClassIdx) + 1"!][!//
    [!ENDLOOP!][!//
    [!WS "$Indent"!]/* Index of failure class configuration */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTFAILURECLASSIDX_MASK & [!"num:i($FailureClassIdx)"!]U) <<[!//
    [!WS!]DEM_EVENTFAILURECLASSIDX_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* Fault confirmation not configured */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_EVENTFAILURECLASSIDX_MASK & DEM_NO_FAULT_CONFIRMATION) <<[!//
    [!WS!]DEM_EVENTFAILURECLASSIDX_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for EventPriority === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_EVENT_PRIORITY", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "../../../../DemGeneral/DemEventDisplacementSupport = 'true'"!][!//
[!/*    configured priority value [1..255] */!][!//
    [!VAR "PriorityValue" = "DemEventClass/DemEventPriority"!][!//
[!/*    get index of configured priority from priority list */!][!//
    [!CALL "GET_INTERNAL_PRIORITY_IDX", "Prio" = "$PriorityValue"!][!//
    [!WS "$Indent"!]/* Internal event priority (0 = most important) */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_PRIORITY_MASK & [!"num:integer($PriorityValueIdx)"!]U) <<[!//
    [!WS!]DEM_PRIORITY_OFFSET )
  [!ELSE!][!//
    [!WS "$Indent"!]/* No event priority used as event displacement is disabled */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)0U << DEM_PRIORITY_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!VAR "DebounceCtrsInitialized"  = "'false'"!][!//
[!/* === Packed EventDesc macro for DebounceAlgo & DebounceIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_DEBOUNCE_ALGORITHM", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
[!IF "$DebounceCtrsInitialized = 'false'"!][!//
[!/* the indexes must be initialized only once and then the macro can be re-called for any number of times */!][!//
  [!/* initialize counting variables */!][!//
  [!VAR "DebounceCtrIdx"  = "0"!][!//
  [!VAR "DebounceTimeIdx" = "0"!][!//
  [!VAR "DebounceFrqIdx"  = "0"!][!//
  [!VAR "DebounceCtrsInitialized"  = "'true'"!][!//
[!ENDIF!][!//
  [!IF "DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceCounterBased'"!][!//
    [!WS "$Indent"!]/* Counter based event debouncing */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DEBOUNCEALGO_MASK & DEM_DEBOUNCE_COUNTERBASED) << DEM_DEBOUNCEALGO_OFFSET )
    [!WS "$Indent"!]/* Index of debouncing configuration/status */
    [!WS "$Indent"!]| ( (uint32)(DEM_DEBOUNCEIDX_MASK & [!"num:i($DebounceCtrIdx)"!]U) << DEM_DEBOUNCEIDX_OFFSET )
    [!VAR "DebounceCtrIdx" = "$DebounceCtrIdx + 1"!][!//
  [!ELSEIF "DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase'"!][!//
    [!WS "$Indent"!]/* Time based event debouncing */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DEBOUNCEALGO_MASK & DEM_DEBOUNCE_TIMEBASED) << DEM_DEBOUNCEALGO_OFFSET )
    [!WS "$Indent"!]/* Index of debouncing configuration/status */
    [!WS "$Indent"!]| ( (uint32)(DEM_DEBOUNCEIDX_MASK & [!"num:i($DebounceTimeIdx)"!]U) << DEM_DEBOUNCEIDX_OFFSET )
    [!VAR "DebounceTimeIdx" = "$DebounceTimeIdx + 1"!][!//
  [!ELSEIF "DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceFrequencyBased'"!][!//
    [!WS "$Indent"!]/* Frequency based event debouncing */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DEBOUNCEALGO_MASK & DEM_DEBOUNCE_FREQUENCYBASED) << DEM_DEBOUNCEALGO_OFFSET )
    [!WS "$Indent"!]/* Index of debouncing configuration/status */
    [!WS "$Indent"!]| ( (uint32)(DEM_DEBOUNCEIDX_MASK & [!"num:i($DebounceFrqIdx)"!]U) << DEM_DEBOUNCEIDX_OFFSET )
    [!VAR "DebounceFrqIdx" = "$DebounceFrqIdx + 1"!][!//
  [!ELSE!][!/* monitor internal debouncing */!][!//
    [!WS "$Indent"!]/* Monitor internal event debouncing */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DEBOUNCEALGO_MASK & DEM_DEBOUNCE_MONITOR) << DEM_DEBOUNCEALGO_OFFSET )
    [!WS "$Indent"!]/* Index not used */
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for DTCFuncUnit === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_FUNCTIONAL_UNIT", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "node:exists(DemDTCClassRef)"!][!//
    [!WS "$Indent"!]/* DTC Functional Unit */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_DTCFUNCTIONALUNIT_MASK & [!"as:ref(DemDTCClassRef)/DemDTCFunctionalUnit"!]U) <<[!//
    [!WS!]DEM_DTCFUNCTIONALUNIT_OFFSET )
  [!ELSE!][!/* No DTC class configured */!][!//
    [!WS "$Indent"!]/* No DTC Functional Unit */
    [!WS "$Indent"!][!"$Prefix"!]( (DEM_DTCFUNCTIONALUNIT_MASK & DEM_NO_DTC_FUNCTIONAL_UNIT) << DEM_DTCFUNCTIONALUNIT_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Packed EventDesc macro for AgingCycleIdx === */!][!//
[!MACRO "EVENT_DESC_BITFIELD_AGING_CYCLE_ID", "Indent", "Prefix" ="'| '"!][!//
[!INDENT "0"!][!//
  [!IF "($CommonOperationCycleEnabled = 'false') and node:exists(DemEventClass/DemAgingCycleRef) and ( name(as:ref(DemEventClass/DemOperationCycleRef)) != name(as:ref(DemEventClass/DemAgingCycleRef)) )"!][!//
    [!WS "$Indent"!]/* Aging cycle Id */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_AGINGCYCLEIDX_MASK & [!"name(as:ref(DemEventClass/DemAgingCycleRef))"!]) <<[!//
    [!WS!]DEM_AGINGCYCLEIDX_OFFSET )
  [!ELSE!][!/* No (different/separate) aging cycle reference configured */!][!//
    [!WS "$Indent"!]/* No aging cycle Id */
    [!WS "$Indent"!][!"$Prefix"!]( (uint32)(DEM_AGINGCYCLEIDX_MASK & DEM_NUM_AGINGCYCLES) << DEM_AGINGCYCLEIDX_OFFSET )
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!//
[!/* === Calculate event description ===
   * Macro CREATE_DTCCLASS_LIST_FROM_EVENTS must be called before!
   */!][!//
[!MACRO "CALC_EVENT_DESC"!][!//
[!INDENT "0"!][!//
  [!/* EvConf1 */!][!//
    [!/* UdsDTC */!][!//
      [!VAR "CalUdsDTCWidth" = "24"!][!//
    [!/* DTCOrigin */!][!//
      [!VAR "CalDTCOriginWidth" = "2"!][!//
    [!/* DTCSeverity */!][!//
      [!VAR "CalDTCSeverityWidth" = "2"!][!//
    [!/* EnablePrestorage */!][!//
      [!IF "$MaxNumPrestoredFF > 0"!][!//
        [!VAR "CalEnablePrestorageWidth" = "1"!][!//
      [!ELSE!][!//
        [!VAR "CalEnablePrestorageWidth" = "0"!][!//
      [!ENDIF!][!//
    [!/* EventKind */!][!//
      [!VAR "CalEventKindWidth" = "1"!][!//
    [!/* IndicatorUsed */!][!//
      [!IF "$NumIndicatorLinks > 0"!][!//
        [!VAR "CalIndicatorWidth" = "1"!][!//
      [!ELSE!][!//
        [!VAR "CalIndicatorWidth" = "0"!][!//
      [!ENDIF!][!//
    [!/* Offset calculation EvConf1 */!][!//
      [!VAR "CalUdsDTCOffset" = "0"!][!//
      [!VAR "CalDTCOriginOffset" = "$CalUdsDTCOffset + $CalUdsDTCWidth"!][!//
      [!VAR "CalDTCSeverityOffset" = "$CalDTCOriginOffset + $CalDTCOriginWidth"!][!//
      [!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_TYPE1'"!][!//
        [!VAR "CalEventKindOffset" = "0"!][!//
        [!VAR "CalIndicatorOffset" = "$CalEventKindOffset + $CalEventKindWidth"!][!//
        [!VAR "CalEnablePrestorageOffset" = "$CalIndicatorOffset + $CalIndicatorWidth"!][!//
        [!VAR "CalImmediateStorageOffset" = "$CalDTCSeverityOffset + $CalDTCSeverityWidth"!][!//
      [!ELSE!][!//
        [!VAR "CalEnablePrestorageOffset" = "$CalDTCSeverityOffset + $CalDTCSeverityWidth"!][!//
        [!VAR "CalEventKindOffset" = "$CalEnablePrestorageOffset + $CalEnablePrestorageWidth"!][!//
        [!VAR "CalIndicatorOffset" = "$CalEventKindOffset + $CalEventKindWidth"!][!//
        [!VAR "CalImmediateStorageOffset" = "$CalIndicatorOffset + $CalIndicatorWidth"!][!//
      [!ENDIF!][!//
  [!/* EvConf2 */!][!//
    [!/* EvSignificance */!][!//
      [!VAR "CalEvDescEvSignWidth" = "1"!][!//
    [!/* CalEvDescFFRecInfo */!][!//
    [!/* FFClassIdx */!][!//
      [!CALL "GET_WIDTH", "PNumber" = "count(DemGeneral/DemFreezeFrameClass/*)", "context" = "'FFClassIdx'"!][!//
      [!VAR "CalEvDescFFClassIdxWidth" = "$NumberOfBits"!][!//
    [!/* CalEvDescFFRecInfo */!][!//
      [!VAR "CalEvDescFFRecInfo" = "0"!][!//
        [!IF "DemGeneral/DemTypeOfFreezeFrameRecordNumeration = 'DEM_FF_RECNUM_CALCULATED'"!][!//
          [!LOOP "DemConfigSet/*/DemEventParameter/*"!][!//
            [!IF "$CalEvDescFFRecInfo < DemMaxNumberFreezeFrameRecords"!][!//
              [!VAR "CalEvDescFFRecInfo" = "DemMaxNumberFreezeFrameRecords"!][!//
            [!ENDIF!][!//
          [!ENDLOOP!][!//
          [!/* calculate the FFRecInfo to include the NULL class as well */!][!//
          [!VAR "CalEvDescFFRecInfo" = "$CalEvDescFFRecInfo + 1"!][!//
        [!ELSE!][!//
          [!VAR "CalEvDescFFRecInfo" = "count(DemGeneral/DemFreezeFrameRecNumClass/*)"!][!//
        [!ENDIF!][!//
      [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescFFRecInfo", "context" = "'CalEvDescFFRecInfo'"!][!//
      [!VAR "CalEvDescFFRecInfoWidth" = "$NumberOfBits"!][!//
    [!/* EDClassIdx */!][!//
      [!CALL "GET_WIDTH", "PNumber" = "count(DemGeneral/DemExtendedDataClass/*)", "context" = "'DemExtendedDataClass'"!][!//
      [!VAR "CalEvDescEDClassIdxWidth" = "$NumberOfBits"!][!//
    [!/* OpCycleIdx */!][!//
      [!VAR "CalEvDescOpCalEvDescCycNameList" = "'#'"!][!//
      [!VAR "CalEvDescOpCycCtr"  = "count(DemGeneral/DemOperationCycle/*)"!][!//
      [!CALL "GET_WIDTH", "PNumber" = "num:integer($CalEvDescOpCycCtr)", "context" = "'CalEvDescOpCycCtr'"!][!//
      [!VAR "CalEvDescOpCycleIdxWidth" = "$NumberOfBits"!][!//
    [!/* AgingCtrThreshold */!][!//
      [!IF "$EventClassSupport = 'false'"!][!//
        [!VAR "CalEvDescMaxAgeCyc" = "0"!][!//
        [!LOOP "DemConfigSet/*/DemEventParameter/*/DemEventClass[DemAgingAllowed = 'true']"!][!//
          [!IF "$CalEvDescMaxAgeCyc < DemAgingCycleCounterThreshold"!][!//
            [!VAR "CalEvDescMaxAgeCyc" = "DemAgingCycleCounterThreshold"!][!//
          [!ENDIF!][!//
        [!ENDLOOP!][!//
        [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescMaxAgeCyc", "context" = "'CalEvDescMaxAgeCyc'"!][!//
        [!VAR "CalEvDescAgingCtrThresholdWidth" = "$NumberOfBits"!][!//
      [!ELSE!][!//
        [!VAR "CalEvDescAgingCtrThresholdWidth" = "0"!][!//
      [!ENDIF!][!//
    [!/* DTC Group */!][!//
      [!VAR "CalEvDescDTCGroupWidth" = "5"!][!//
    [!/* Event Class Index */!][!//
      [!IF "$EventClassSupport = 'true'"!][!//
        [!CALL "CREATE_EVENT_CLASS_LIST"!][!//
        [!VAR "CalEvDescNumberOfEventClasses" = "count(text:split($EventClassList, '#'))"!][!//
        [!VAR "CalEvDescMaxNumberEventClasses" = "/AUTOSAR/TOP-LEVEL-PACKAGES/Dem/ELEMENTS/Dem/DemGeneral/DemMaxNumberEventClasses"!][!//
        [!IF "$CalEvDescNumberOfEventClasses < $CalEvDescMaxNumberEventClasses"!][!//
          [!VAR "CalEvDescNumberOfEventClasses" = "$CalEvDescMaxNumberEventClasses"!][!//
        [!ENDIF!][!//
        [!CALL "GET_WIDTH", "PNumber" = "(num:integer($CalEvDescNumberOfEventClasses) - 1)", "context" = "'CalEvDescNumberOfEventClasses'"!][!//
        [!VAR "CalEvDescEventClassIdxWidth" = "$NumberOfBits"!][!//
      [!ELSE!][!//
        [!VAR "CalEvDescEventClassIdxWidth" = "0"!][!//
      [!ENDIF!][!//
  [!/* EvConf3 */!][!//
    [!/* EnCondGrpIdx */!][!//
      [!VAR "CalEvDescNumEnCondGrp" = "num:i(count(DemGeneral/DemEnableConditionGroup/*))"!][!//
      [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescNumEnCondGrp", "context" = "'CalEvDescNumEnCondGrp'"!][!//
      [!VAR "CalEvDescEnCondGrpIdxWidth" = "$NumberOfBits"!][!//
    [!/* EventFailureClassIdx */!][!//
      [!VAR "CalEvDescFailureCycleCfgList" = "'#'"!][!//
      [!VAR "CalEvDescEventFailureClassIdx" = "0"!][!//
        [!LOOP "(DemConfigSet/*/DemEventParameter/*[node:exists(./DemEventClass/DemEventFailureCycleRef)])"!][!//
          [!VAR "CalEvDescEventFailureCycle" = "node:name(as:ref(DemEventClass/DemEventFailureCycleRef))"!][!//
          [!VAR "CalEvDescEventFailureThreshold" = "node:value(DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
          [!IF "not(contains($CalEvDescFailureCycleCfgList, concat('#', $CalEvDescEventFailureCycle, '*', $CalEvDescEventFailureThreshold, '#')))"!][!//
            [!/* add non-existing cycle counter cycle and threshold to the list */!][!//
            [!VAR "CalEvDescFailureCycleCfgList" = "concat($CalEvDescFailureCycleCfgList, $CalEvDescEventFailureCycle, '*', $CalEvDescEventFailureThreshold, '#')"!][!//
            [!/* increment counter of unique combinations */!][!//
            [!VAR "CalEvDescEventFailureClassIdx" = "$CalEvDescEventFailureClassIdx + 1"!][!//
          [!ENDIF!][!//
        [!ENDLOOP!][!//
      [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescEventFailureClassIdx", "context" = "'CalEvDescEventFailureClassIdx'"!][!//
      [!VAR "CalEvDescEventFailureClassIdxWidth" = "$NumberOfBits"!][!//
    [!/* Priority */!][!//
      [!IF "$EventClassSupport = 'false'"!][!//
        [!VAR "CalEvDescNumPriorities" = "0"!][!//
        [!IF "(DemGeneral/DemEventDisplacementSupport = 'true') or (count(as:modconf('Dem')[1]/DemConfigSet/*/DemPidClass/*) > 0)"!][!//
          [!VAR "CalEvDescPriorityValueList" = "'#'"!][!//
          [!LOOP "DemConfigSet/*/DemEventParameter/*/DemEventClass"!][!//
            [!IF "not(contains($CalEvDescPriorityValueList, concat('#', DemEventPriority, '#')))"!][!//
              [!VAR "CalEvDescPriorityValueList" = "concat($CalEvDescPriorityValueList, DemEventPriority, '#')"!][!//
            [!ENDIF!][!//
          [!ENDLOOP!][!//
          [!VAR "CalEvDescNumPriorities" = "count(text:split($CalEvDescPriorityValueList, '#'))"!][!//
        [!ENDIF!][!//
        [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescNumPriorities - 1", "context" = "'CalEvDescNumPriorities'"!][!//
        [!VAR "CalEvDescPriorityWidth" = "$NumberOfBits"!][!//
      [!ELSE!][!//
        [!VAR "CalEvDescPriorityWidth" = "0"!][!//
      [!ENDIF!][!//
    [!/* DebounceAlgo */!][!//
      [!VAR "CalEvDescDebounceAlgoWidth" = "2"!][!//
    [!/* DebounceIdx */!][!//
    [!VAR "CalEvDescMaxDebIdx" = "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceCounterBased'])"!][!//
      [!VAR "CalEvDescMaxTimeDebIdx" = "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase'])"!][!//
      [!IF "$CalEvDescMaxDebIdx < $CalEvDescMaxTimeDebIdx"!][!//
        [!VAR "CalEvDescMaxDebIdx" = "$CalEvDescMaxTimeDebIdx"!][!//
      [!ENDIF!][!//
      [!VAR "CalEvDescMaxFreqDebIdx" = "count(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceFrequencyBased'])"!][!//
      [!IF "$CalEvDescMaxDebIdx < $CalEvDescMaxFreqDebIdx"!][!//
        [!VAR "CalEvDescMaxDebIdx" = "$CalEvDescMaxFreqDebIdx"!][!//
      [!ENDIF!][!//
      [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescMaxDebIdx", "context" = "'CalEvDescMaxDebIdx'"!][!//
      [!VAR "CalEvDescDebounceIdxWidth" = "$NumberOfBits"!][!//
    [!/* DTCFuncUnit */!][!//
      [!VAR "CalEvDescMaxDTCFuncUnit" = "0"!][!//
      [!LOOP "DemConfigSet/*/DemDTCClass/*"!][!//
        [!IF "$CalEvDescMaxDTCFuncUnit < DemDTCFunctionalUnit"!][!//
          [!VAR "CalEvDescMaxDTCFuncUnit" = "DemDTCFunctionalUnit"!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
      [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescMaxDTCFuncUnit", "context" = "'CalEvDescMaxDTCFuncUnit'"!][!//
      [!VAR "CalEvDescDTCFuncUnitWidth" = "$NumberOfBits"!][!//
    [!/* Aging cycle index */!][!//
      [!IF "$CommonOperationCycleEnabled = 'false'"!][!//
        [!VAR "CalEvDescCycNameList" = "'#'"!][!//
        [!/* additional value "not configured" is added */!][!//
        [!VAR "CalEvDescAgeCycCtr" = "count(DemGeneral/DemAgingCycle/*) + 1"!][!//
        [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescAgeCycCtr", "context" = "'CalEvDescAgeCycCtr'"!][!//
        [!VAR "CalEvDescAgeCycleIdxWidth" = "$NumberOfBits"!][!//
      [!ELSE!][!//
        [!VAR "CalEvDescAgeCycleIdxWidth" = "0"!][!//
      [!ENDIF!][!//
  [!/* EvConf4 */!][!//
    [!/* MultiEventTriggeringClassIdx */!][!//
      [!CALL "CALC_MULTI_EVENT_CLASSES"!][!//
      [!CALL "GET_MULTI_EVENT_TRIGGERING_CLASS_COUNT"!][!//
      [!VAR "CalEvDescNumMultiEvTrigClasses" = "$NumMultiEventTriggeringClasses"!][!//
      [!CALL "GET_WIDTH", "PNumber" = "$CalEvDescNumMultiEvTrigClasses", "context" = "'CalEvDescNumMultiEvTrigClasses'"!][!//
      [!VAR "CalEvDescMultiEvTrigClassIdxWidth" = "$NumberOfBits"!][!//
[!//
      [!IF "DemGeneral/DemEventCombinationSupport = 'DEM_EVCOMB_DISABLED'"!][!//
        [!/* Offset calculation EvConf2 */!][!//
        [!VAR "CalEvDescEvSignOffset" = "0"!][!//
        [!VAR "CalEvDescFFClassIdxOffset" = "$CalEvDescEvSignOffset + $CalEvDescEvSignWidth"!][!//
        [!VAR "CalEvDescFFRecInfoOffset" = "$CalEvDescFFClassIdxOffset + $CalEvDescFFClassIdxWidth"!][!//
        [!VAR "CalEvDescEDClassIdxOffset" = "$CalEvDescFFRecInfoOffset + $CalEvDescFFRecInfoWidth"!][!//
        [!VAR "CalEvDescOpCycleIdxOffset" = "$CalEvDescEDClassIdxOffset + $CalEvDescEDClassIdxWidth"!][!//
        [!VAR "CalEvDescAgingCtrThresholdOffset" = "$CalEvDescOpCycleIdxOffset + $CalEvDescOpCycleIdxWidth"!][!//
        [!VAR "CalEvDescDTCGroupOffset" = "$CalEvDescAgingCtrThresholdOffset + $CalEvDescAgingCtrThresholdWidth"!][!//
        [!VAR "CalEvDescEventClassIdxOffset" = "$CalEvDescDTCGroupOffset + $CalEvDescDTCGroupWidth"!][!//
        [!VAR "CalEvDescEvConf2Size" = "$CalEvDescEventClassIdxOffset + $CalEvDescEventClassIdxWidth"!][!//
        [!IF "$CalEvDescEvConf2Size > 32"!][!//
          [!ERROR "Event-Configuration-Layout Part Nr. 2 needs too much space"!][!//
        [!ENDIF!][!//
        [!/* Offset calculation EvConf3 */!][!//
        [!VAR "CalEvDescEnCondGrpIdxOffset" = "0"!][!//
        [!VAR "CalEvDescEventFailureClassIdxOffset" = "$CalEvDescEnCondGrpIdxOffset + $CalEvDescEnCondGrpIdxWidth"!][!//
        [!VAR "CalEvDescPriorityOffset" = "$CalEvDescEventFailureClassIdxOffset + $CalEvDescEventFailureClassIdxWidth"!][!//
        [!VAR "CalEvDescDebounceAlgoOffset" = "$CalEvDescPriorityOffset + $CalEvDescPriorityWidth"!][!//
        [!VAR "CalEvDescDebounceIdxOffset" = "$CalEvDescDebounceAlgoOffset + $CalEvDescDebounceAlgoWidth"!][!//
        [!VAR "CalEvDescDTCFuncUnitOffset" = "$CalEvDescDebounceIdxOffset + $CalEvDescDebounceIdxWidth"!][!//
        [!VAR "CalEvDescAgeCycleIdxOffset" = "$CalEvDescDTCFuncUnitOffset + $CalEvDescDTCFuncUnitWidth"!][!//
        [!VAR "CalEvDescEvConf3Size" = "$CalEvDescAgeCycleIdxOffset + $CalEvDescAgeCycleIdxWidth"!][!//
        [!IF "$CalEvDescEvConf3Size > 32"!][!//
          [!ERROR "Event-Configuration-Layout Part Nr. 3 needs too much space"!][!//
        [!ENDIF!][!//
        [!/* Offset calculation EvConf4 */!][!//
        [!VAR "CalEvDescMultiEvTrigClassIdxOffset" = "0"!][!//
        [!VAR "CalEvDescEvConf4Size" = "$CalEvDescMultiEvTrigClassIdxOffset + $CalEvDescMultiEvTrigClassIdxWidth"!][!//
        [!IF "$CalEvDescEvConf4Size > 16"!][!//
          [!ERROR "Event-Configuration-Layout Part Nr. 4 needs too much space"!][!//
        [!ENDIF!][!//
      [!ELSE!][!//
        [!/* Offset calculation DTCConf2 */!][!//
        [!VAR "CalEvDescPriorityOffset" = "0"!][!//
        [!VAR "CalEvDescFFClassIdxOffset" = "$CalEvDescPriorityOffset + $CalEvDescPriorityWidth"!][!//
        [!VAR "CalEvDescFFRecInfoOffset" = "$CalEvDescFFClassIdxOffset + $CalEvDescFFClassIdxWidth"!][!//
        [!VAR "CalEvDescEDClassIdxOffset" = "$CalEvDescFFRecInfoOffset + $CalEvDescFFRecInfoWidth"!][!//
        [!VAR "CalEvDescAgeCycleIdxOffset" = "$CalEvDescEDClassIdxOffset + $CalEvDescEDClassIdxWidth"!][!//
        [!VAR "CalEvDescAgingCtrThresholdOffset" = "$CalEvDescAgeCycleIdxOffset + $CalEvDescAgeCycleIdxWidth"!][!//
        [!VAR "CalEvDescDTCFuncUnitOffset" = "$CalEvDescAgingCtrThresholdOffset + $CalEvDescAgingCtrThresholdWidth"!][!//
        [!VAR "CalEvDescDTCGroupOffset" = "$CalEvDescDTCFuncUnitOffset + $CalEvDescDTCFuncUnitWidth "!][!//
        [!VAR "CalEvDescEvSignOffset" = "$CalEvDescDTCGroupOffset + $CalEvDescDTCGroupWidth"!][!//
        [!VAR "CalDTCDescEvConf2Size" = "$CalEvDescEvSignOffset + $CalEvDescEvSignWidth"!][!//
        [!IF "$CalDTCDescEvConf2Size > 32"!][!//
            [!ERROR "DTC-Configuration-Layout(Dem_DTCDescType->DTCConf2) Part Nr. 2 needs too much space"!][!//
        [!ENDIF!][!//
        [!/* Width calculation EvConf1 */!][!//
        [!/*
           * Offset calculation Dem_EventDesc depends on DemNbOfDTCClasses, generated
           * by the CREATE_DTCCLASS_LIST_FROM_EVENTS macro
           */!][!//
        [!CALL "GET_WIDTH", "PNumber" = "$DemNbOfDTCClasses", "context" = "'CalEvDescDTCConfIdxWidth'"!][!//
        [!VAR "CalEvDescDTCConfIdxWidth" = "$NumberOfBits"!][!//
        [!/* Width calculation EvConf2 */!][!//
        [!/* Offset calculation EvConf1 */!][!//
        [!VAR "CalEvDescDebounceAlgoOffset" = "$CalEnablePrestorageWidth + $CalIndicatorWidth + 1"!][!//
        [!VAR "CalEvDescDebounceIdxOffset" = "$CalEvDescDebounceAlgoOffset + 2"!][!//
        [!VAR "CalEvDescEnCondGrpIdxOffset" = "$CalEvDescDebounceIdxOffset + $CalEvDescDebounceIdxWidth"!][!//
        [!VAR "CalEvDescEventFailureClassIdxOffset" = "$CalEvDescEnCondGrpIdxOffset + $CalEvDescEnCondGrpIdxWidth"!][!//
        [!VAR "CalEvDescOpCycleIdxOffset" = "$CalEvDescEventFailureClassIdxOffset + $CalEvDescEventFailureClassIdxWidth"!][!//
        [!VAR "CalEvDescDTCConfIdxOffset" = "$CalEvDescOpCycleIdxOffset + $CalEvDescOpCycleIdxWidth"!][!//
        [!VAR "CalEventDescSize" = "$CalEvDescDTCConfIdxOffset + $CalEvDescDTCConfIdxWidth"!][!//
        [!IF "$CalEventDescSize > 32"!][!//
            [!ERROR "Event-Configuration-Layout(Dem_EventDescType) needs too much space"!][!//
        [!ENDIF!][!//
        [!/* Offset calculation EvConf2 */!][!//
        [!VAR "CalEvDescMultiEvTrigClassIdxOffset" = "0"!][!//
        [!VAR "CalEvDescEvConf4Size" = "$CalEvDescMultiEvTrigClassIdxOffset + $CalEvDescMultiEvTrigClassIdxWidth"!][!//
        [!IF "$CalEvDescEvConf4Size > 16"!][!//
          [!ERROR "Event-Configuration-Layout Part Nr. 2 needs too much space"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* OBD_EVENT_OFFSET calculates offsets values for OBD */!][!//
[!MACRO "OBD_EVENT_OFFSET"!][!//
[!INDENT "0"!][!//
  [!IF "DemGeneral/DemOBDSupport = 'true'"!][!//
    [!/* Width of DEM_MINRATIOEVENT_WIDTH */!][!//
    [!VAR "CalMinRatioEventWidth" = "1"!][!//
    [!IF "node:exists(DemGeneral/DemMaxNumEventsPerReadinessGroup)"!]
      [!IF "num:integer(DemGeneral/DemMaxNumEventsPerReadinessGroup) = '0'"!][!//
        [!VAR "CalOBDReadinessWidth" = "4"!][!//
      [!ELSE!][!//
        [!VAR "CalOBDReadinessWidth" = "0"!][!//
      [!ENDIF!][!//
    [!ELSE!][!//
      [!VAR "CalOBDReadinessWidth" = "4"!][!//
    [!ENDIF!][!//
    [!VAR "CalMinRatioEventOffset" = "0"!][!//
    [!VAR "CalOBDReadinessOffset" = "$CalMinRatioEventOffset + $CalMinRatioEventWidth"!][!//
    [!VAR "CalOBDAgingCycCntThresholdOffset" = "$CalOBDReadinessOffset + $CalOBDReadinessWidth"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
*
* CREATE_FAILURE_COUNTER_CYCLE_LIST creates a list of unique failure class cycles and thresholds
*/!][!//
[!MACRO "CREATE_FAILURE_COUNTER_CYCLE_LIST"!][!//
[!INDENT "0"!][!//
  [!/* create unique list of all configured
   * failure cycle counter cycles and thresholds */!][!//
  [!VAR "FailureCycleCfgList" = "'#'"!][!//
  [!VAR "EventFailureClassIdx" = "0"!][!//
  [!VAR "EventFailureCycle" = "''"!][!//
  [!LOOP "(DemConfigSet/*/DemEventParameter/*[node:exists(./DemEventClass/DemEventFailureCycleCounterThreshold)])"!][!//
    [!IF "$CommonOperationCycleEnabled = 'false'"!][!//
      [!VAR "EventFailureCycle" = "node:name(as:ref(DemEventClass/DemEventFailureCycleRef))"!][!//
    [!ELSE!][!//
      [!VAR "EventFailureCycle" = "node:name(as:ref(DemEventClass/DemOperationCycleRef))"!][!//
    [!ENDIF!][!//
    [!VAR "EventFailureThreshold" = "node:value(DemEventClass/DemEventFailureCycleCounterThreshold)"!][!//
    [!IF "not(contains($FailureCycleCfgList, concat('#', $EventFailureCycle, '*', $EventFailureThreshold, '#')))"!][!//
[!/* add non-existing cycle counter cycle and threshold to the list */!][!//
      [!VAR "FailureCycleCfgList" = "concat($FailureCycleCfgList, $EventFailureCycle, '*', $EventFailureThreshold, '#')"!][!//
[!/* increment counter of unique combinations */!][!//
      [!VAR "EventFailureClassIdx" = "$EventFailureClassIdx + 1"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Get debounce classes === */!][!//
[!VAR "DebounceCounterConsecutiveCallFlag"  = "'false'"!][!//
[!MACRO "DEBOUNCE_COUNTER_CLASS_CALC"!][!//
[!INDENT "0"!][!//
  [!IF "$DebounceCounterConsecutiveCallFlag = 'false'"!][!// 
    [!VAR "DebounceClasses" = "''"!][!//
    [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceCounterBased'], 'DemEventId')"!][!//
      [!SELECT "DemEventClass/DemDebounceAlgorithmClass"!][!//
        [!VAR "UnconfirmedThreshold" = "0"!][!//
        [!IF "node:exists(DemDebounceCounterUnconfirmedThreshold) = 'true'"!][!//
          [!VAR "UnconfirmedThreshold" = "DemDebounceCounterUnconfirmedThreshold"!][!//
        [!ENDIF!][!//
        [!VAR "DebounceClass" = "concat('#', DemDebounceCounterDecrementStepSize, ':', DemDebounceCounterIncrementStepSize, ':', DemDebounceCounterPassedThreshold, ':', DemDebounceCounterFailedThreshold, ':', $UnconfirmedThreshold, ':', DemDebounceCounterJumpDownValue, ':', DemDebounceCounterJumpUpValue, ':', DemDebounceBehavior, ':', DemDebounceCounterJumpDown, ':', DemDebounceCounterJumpUp, ':', DemDebounceCounterStorage, '#')"!][!//
        [!IF "not(contains($DebounceClasses, $DebounceClass))"!][!//
          [!VAR "DebounceClasses" = "concat($DebounceClasses, $DebounceClass)"!][!//
        [!ENDIF!][!//
      [!ENDSELECT!][!//
    [!ENDLOOP!][!//
    [!VAR "DebounceCounterConsecutiveCallFlag"  = "'true'"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Number of debounce classes ===
   * Macro DEBOUNCE_COUNTER_CLASS_CALC must be called before!
   */!][!//
[!MACRO "DEBOUNCE_COUNTER_GET_CLASS_COUNT"!][!//
[!CALL "DEBOUNCE_COUNTER_CLASS_CALC"!]
[!INDENT "0"!][!//
  [!VAR "DebounceCounterClassCount" = "count(text:split($DebounceClasses, '#'))"!][!//
[!ENDINDENT!][!//
[!IF "node:exists(DemGeneral/DemMaxNumberDebCounterClasses)"!]
  [!IF "$DebounceCounterClassCount < DemGeneral/DemMaxNumberDebCounterClasses"!]
    [!VAR "DebounceCounterClassCount" = "DemGeneral/DemMaxNumberDebCounterClasses"!]
  [!ENDIF!]
[!ENDIF!]
[!ENDMACRO!][!//
[!//
[!/* === Get debounce class index ===
   * Macro DEBOUNCE_COUNTER_CLASS_CALC must be called before!
   */!][!//
[!MACRO "DEBOUNCE_COUNTER_GET_CLASS_INDEX", "EventId"!][!//
[!INDENT "0"!][!//
  [!VAR "UnconfirmedThreshold" = "0"!][!//
  [!IF "node:exists(DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterUnconfirmedThreshold) = 'true'"!][!//
    [!VAR "UnconfirmedThreshold" = "DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterUnconfirmedThreshold"!][!//
  [!ENDIF!][!//
  [!VAR "DebounceCounterClassIndex" = "0"!][!//
  [!VAR "DebounceClass" = "concat(DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterDecrementStepSize, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterIncrementStepSize, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterPassedThreshold, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterFailedThreshold, ':', $UnconfirmedThreshold, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterJumpDownValue, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterJumpUpValue, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceBehavior, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterJumpDown, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterJumpUp, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceCounterStorage)"!][!//
  [!IF "contains($DebounceClasses, $DebounceClass)"!][!//
    [!LOOP "text:split($DebounceClasses, '#')"!][!//
      [!IF ". = $DebounceClass"!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
      [!VAR "DebounceCounterClassIndex" = "$DebounceCounterClassIndex + 1"!][!//
    [!ENDLOOP!][!//
    [!VAR "DebounceCounterClassIndex" = "concat(num:i($DebounceCounterClassIndex), 'U')"!][!//
  [!ELSE!][!//
    [!VAR "DebounceCounterClassIndex" = "'DEM_DEBOUNCE_CLASS_INVALID'"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!//
[!//
[!/* === Get Time debounce classes === */!][!//
[!VAR "DebounceTimeConsecutiveCallFlag"  = "'false'"!][!//
[!MACRO "DEBOUNCE_TIME_CLASS_CALC"!][!//
[!INDENT "0"!][!//
  [!IF "$DebounceTimeConsecutiveCallFlag = 'false'"!][!// 
    [!VAR "TimeDebounceClasses" = "''"!][!//
    [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*[DemEventClass/DemDebounceAlgorithmClass = 'DemDebounceTimeBase'], 'DemEventId')"!][!//
      [!SELECT "DemEventClass/DemDebounceAlgorithmClass"!][!//
        [!VAR "TimeDebounceClass" = "concat('#', DemDebounceTimeFailedThreshold, ':', DemDebounceTimePassedThreshold, ':', DemDebounceBehavior, '#')"!][!//
        [!IF "not(contains($TimeDebounceClasses, $TimeDebounceClass))"!][!//
          [!VAR "TimeDebounceClasses" = "concat($TimeDebounceClasses, $TimeDebounceClass)"!][!//
        [!ENDIF!][!//
      [!ENDSELECT!][!//
    [!ENDLOOP!][!//
	[!VAR "DebounceTimeConsecutiveCallFlag"  = "'true'"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Number of Time debounce classes ===
   * Macro DEBOUNCE_TIME_CLASS_CALC must be called before!
   */!][!//
[!MACRO "DEBOUNCE_TIME_GET_CLASS_COUNT"!][!//
[!CALL "DEBOUNCE_TIME_CLASS_CALC"!]
[!INDENT "0"!][!//
  [!VAR "DebounceTimeClassCount" = "count(text:split($TimeDebounceClasses, '#'))"!][!//
[!ENDINDENT!][!//
[!IF "node:exists(DemGeneral/DemMaxNumberDebTimeClasses)"!]
  [!IF "$DebounceTimeClassCount < DemGeneral/DemMaxNumberDebTimeClasses"!]
    [!VAR "DebounceTimeClassCount" = "DemGeneral/DemMaxNumberDebTimeClasses"!]
  [!ENDIF!]
[!ENDIF!]
[!ENDMACRO!][!//
[!//
[!/* === Get Time debounce class index ===
   * Macro DEBOUNCE_TIME_CLASS_CALC must be called before!
   */!][!//
[!MACRO "DEBOUNCE_TIME_GET_CLASS_INDEX", "EventId"!][!//
[!INDENT "0"!][!//
  [!VAR "DebounceTimeClassIndex" = "0"!][!//
  [!VAR "TimeDebounceClass" = "concat(DemEventClass/DemDebounceAlgorithmClass/DemDebounceTimeFailedThreshold, ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceTimePassedThreshold,  ':', DemEventClass/DemDebounceAlgorithmClass/DemDebounceBehavior)"!][!//
  [!IF "contains($TimeDebounceClasses, $TimeDebounceClass)"!][!//
    [!LOOP "text:split($TimeDebounceClasses, '#')"!][!//
      [!IF ". = $TimeDebounceClass"!][!//
        [!BREAK!][!//
      [!ENDIF!][!//
      [!VAR "DebounceTimeClassIndex" = "$DebounceTimeClassIndex + 1"!][!//
    [!ENDLOOP!][!//
    [!VAR "DebounceTimeClassIndex" = "concat(num:i($DebounceTimeClassIndex), 'U')"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!VAR "AvailabilitySupport" = "node:exists(DemGeneral/DemAvailabilitySupport) and (DemGeneral/DemAvailabilitySupport = 'DEM_EVENT_AVAILABILITY')"!]
[!//
[!/* === Create a filled list of events per readiness group === */!][!//
[!MACRO "GET_EVENTS_IN_READINESSGROUP", "GroupName"!][!//
  [!VAR "NumEventsInRdyGrp" = "0"!][!//
  [!INDENT "4"!][!//
  /* [!"$GroupName"!] */
  {
    [!INDENT "6"!][!//
    [!/* add events that are assigned to this readiness group */!][!//
    [!LOOP "DemConfigSet/*/DemEventParameter/*[DemEventClass/DemEventOBDReadinessGroup = $GroupName]"!][!//
      DemConf_DemEventParameter_[!"node:name(.)"!],
      [!VAR "NumEventsInRdyGrp" = "$NumEventsInRdyGrp + 1"!][!//
    [!ENDLOOP!][!//
    [!/* fill the array up to the maximum number of events per group */!][!//
    [!FOR "UnusedEventsInRdyGrp" = "$NumEventsInRdyGrp + 1" TO "node:value(DemGeneral/DemMaxNumEventsPerReadinessGroup)"!][!//
      DEM_EVENT_ID_INVALID,
    [!ENDFOR!][!//
    [!ENDINDENT!][!//
  },
  [!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Get the internal priority index === */!][!//
[!MACRO "GET_INTERNAL_PRIORITY_IDX", "Prio"!][!//
[!INDENT "0"!][!//
[!/* internally packed priority value [0..(N-1)] */!][!//
  [!VAR "PriorityValueIdx" = "0"!][!//
  [!LOOP "text:split($PriorityValueList, '#')"!][!//
    [!IF ". = $Prio"!][!//
      [!BREAK!][!//
    [!ELSE!][!//
      [!VAR "PriorityValueIdx" = "$PriorityValueIdx + 1"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Create a filled list of internal priority values === */!][!//
[!MACRO "CREATE_PRIORITY_VALUE_LIST"!][!//
[!INDENT "0"!][!//
  [!VAR "PriorityValueList" = "'#'"!][!//
  [!LOOP "node:order(DemConfigSet/*/DemEventParameter/*[node:exists(DemEventClass/DemEventPriority)]/DemEventClass, 'DemEventPriority')"!][!//
    [!IF "not(contains($PriorityValueList, concat('#', DemEventPriority, '#')))"!][!//
      [!/* add non-existing higher priority value at the end of the list */!][!//
      [!VAR "PriorityValueList" = "concat($PriorityValueList, DemEventPriority, '#')"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!MACRO "GET_NUMBER_OF_EVENT_PRIORITIES"!][!//
  [!CALL "CREATE_PRIORITY_VALUE_LIST"!][!//
  [!VAR "NumberOfPriorities" = "num:i(count(text:split($PriorityValueList, '#')))"!][!//
[!ENDMACRO!][!//
[!//
[!/* Creates lists used to generate events and DTC data arrays, for event combination */!][!//
[!MACRO "CREATE_DTCCLASS_LIST_FROM_EVENTS"!][!//
[!INDENT "0"!][!//
[!/* === DemDTCClassList ===
   * This list contains combined DTC values, non-combined DTC values, and events without DTC in this order.
   * The DemDTCClassList is used for generating the Dem_DTCDesc[] config array.
   * If DemEventCombinationSupport is enabled (DEM_EVCOMB_TYPE1), Dem_EventDesc[] entries contain an
   * index to a Dem_DTCDesc[] entry which needs to be generated for events without DTC as well.
   * Note that the SHORT-NAME-path of the DTC and event is unique within the SHORT-NAME parent
   * and therefore DemEventsWithoutDTCList can be appended to DemDTCClassList.
   */!][!//
  [!VAR "DemDTCClassList" = "'#'"!][!//
  [!VAR "DemNonCombinedDTCList" = "'#'"!][!//
  [!VAR "DemEventsWithoutDTCList" = "'#'"!][!//
  [!VAR "DemCmbDTCEvLnkList" = "'#'"!][!//
  [!VAR "DemIdxInCmbDTCEvents" = "0"!][!//
  [!VAR "DemCmbDTCEventsList" = "'#'"!][!//
[!//
  [!LOOP "as:modconf('Dem')[1]/DemConfigSet/*/DemDTCClass/*"!][!//
    [!VAR "DTCClassName" = "node:name(.)"!][!//
    [!VAR "NumOfEventsWithTheSameDTC" = "0"!][!//
    [!VAR "DemAuxCmbDTCEvLnkList" = "'#'"!][!//
    [!VAR "EvCombParentEventId" = "0"!][!//
[!/* for performance reasons, looping is not performed in DemEventId order (not needed) */!][!//
    [!LOOP "as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*[node:exists(DemDTCClassRef)]"!][!//
[!/* for performance reasons, the name (always unique) is extracted from the SHORT-NAME-path string
   * instead of using the node function "name(as:ref(DemDTCClassRef))" */!][!//
      [!IF "$DTCClassName = text:split(DemDTCClassRef, '/')[last()]"!][!//
        [!VAR "NumOfEventsWithTheSameDTC" = "$NumOfEventsWithTheSameDTC + 1"!][!//
        [!IF "node:exists(DemEvtCmbCommonParamMaster) and (DemEvtCmbCommonParamMaster = 'true')"!][!//
          [!VAR "EvCombParentEventId" = "node:value(DemEventId)"!][!//
        [!ELSE!][!//
          [!VAR "DemAuxCmbDTCEvLnkList" = "concat($DemAuxCmbDTCEvLnkList, node:value(DemEventId), '#')"!][!//
        [!ENDIF!][!//
      [!ENDIF!][!//
    [!ENDLOOP!][!//
    [!IF "$NumOfEventsWithTheSameDTC > 1"!][!//
      [!VAR "DemDTCClassList" = "concat($DemDTCClassList, $DTCClassName, '#')"!][!//
      [!VAR "DemCmbDTCEvLnkList" = "concat($DemCmbDTCEvLnkList, $DemIdxInCmbDTCEvents, '#')"!][!//
      [!VAR "DemIdxInCmbDTCEvents" = "($DemIdxInCmbDTCEvents + $NumOfEventsWithTheSameDTC)"!][!//
      [!VAR "DemCmbDTCEventsList" = "concat($DemCmbDTCEventsList , $EvCombParentEventId, '#')"!][!//
      [!VAR "DemCmbDTCEventsList" = "concat($DemCmbDTCEventsList , $DemAuxCmbDTCEvLnkList, '#')"!][!//
    [!ELSEIF "$NumOfEventsWithTheSameDTC = 1"!][!//
      [!VAR "DemNonCombinedDTCList" = "concat($DemNonCombinedDTCList, $DTCClassName, '#')"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!/* Create the list of events without associated DTC */!][!//
  [!LOOP "as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*[not(node:exists(DemDTCClassRef))]"!][!//
    [!VAR "DemEventsWithoutDTCList" = "concat($DemEventsWithoutDTCList, node:name(.), '#')"!][!//
  [!ENDLOOP!][!//
[!//
[!/* Additional entry is created in order to calculate the number of linked events */!][!//
  [!VAR "DemCmbDTCEvLnkList" = "concat($DemCmbDTCEvLnkList, $DemIdxInCmbDTCEvents, '#')"!][!//
  [!VAR "DemDTCClassList" = "concat($DemDTCClassList, $DemNonCombinedDTCList, '#')"!][!//
  [!VAR "DemDTCClassList" = "concat($DemDTCClassList, $DemEventsWithoutDTCList, '#')"!][!//
  [!VAR "DemNbOfDTCClasses" = "count(text:split($DemDTCClassList, '#'))"!][!//
  [!VAR "DemNbOfCmbDTCs" = "(count(text:split($DemCmbDTCEvLnkList, '#')) - 1)"!][!//
  [!VAR "DemNbOfCmbEvents" = "count(text:split($DemCmbDTCEventsList, '#'))"!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* === Create a macro for preparing the BSW distribution settings ===
   * This macro must be called once at the begining of the file!
   */!][!//
[!MACRO "CALCULATE_MULTICORE_SETTINGS"!][!//
[!INDENT "0"!][!//
  [!VAR "DemBSWDistributionOfEventReports" = "'false'"!][!//
  [!IF "node:exists(DemGeneral/DemBSWDistribution/DemDistributedBSWEventReporting) and (DemGeneral/DemBSWDistribution/DemDistributedBSWEventReporting = 'true')"!][!//
    [!VAR "DemBSWDistributionOfEventReports" = "'true'"!][!//
  [!ENDIF!][!//
[!//
  [!IF "$DemBSWDistributionOfEventReports = 'true'"!][!//
    [!VAR "NumberOfSatteliteCores" = "count(DemGeneral/DemBSWDistribution/DemSatelliteInstanceConfig/*)"!][!//
  [!ELSE!][!//
    [!VAR "NumberOfSatteliteCores" = "'0'"!][!//
  [!ENDIF!][!//
[!//
  [!IF "node:exists(DemGeneral/DemBSWDistribution/DemMasterCoreId)"!][!//
    [!VAR "MasterCoreId" = "num:i(DemGeneral/DemBSWDistribution/DemMasterCoreId)"!][!//
  [!ELSE!][!//
    [!VAR "MasterCoreId" = "'0'"!][!//
  [!ENDIF!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
* === Multi-event triggering Classes ===
* Calculates all multi-event triggering classes
*
* Classes are separated by #
* Class MultiEventTriggeringClassList entries are separated by ' '
*/!][!//
[!MACRO "CALC_MULTI_EVENT_CLASSES"!][!//
[!INDENT "0"!][!//
  [!/* create unique list of all configured multi-triggering elements */!][!//
  [!VAR "ClassIndex" = "0"!][!//
  [!VAR "MultiEventTriggeringSlaveGroupList" = "'#'"!][!//
  [!VAR "MultiEventTriggeringClassList" = "'#'"!][!//
  [!VAR "MultiEventTriggeringClassIndexList" = "'#'"!][!//
  [!VAR "SlaveIdx" = "0"!][!//
  [!LOOP "node:order(DemGeneral/DemMultiEventTriggering/*, 'node:ref(DemMultiEventTriggeringMasterEventRef)/DemEventId')"!][!//
    [!VAR "Behavior" = "./DemMultiEventTriggeringAlternativeBehavior"!][!//
    [!VAR "MasterId" = "node:ref(DemMultiEventTriggeringMasterEventRef)/DemEventId"!][!//
    [!VAR "SlaveCount" = "0"!][!//
    [!VAR "SlaveEventsGroup" = "concat($Behavior, ';')"!][!//
    [!/* get each class of slave events with their behavior */!][!//
    [!LOOP "node:order(./DemMultiEventTriggeringSlaveEventRef/*, 'as:ref(.)/DemEventId')"!][!//
      [!VAR "SlaveCount" = "$SlaveCount + 1"!][!//
      [!VAR "CurrentSlaveId" = "as:ref(.)/DemEventId"!][!//
      [!VAR "SlaveEventsGroup" = "concat($SlaveEventsGroup, $CurrentSlaveId, ' ')"!][!//
    [!ENDLOOP!][!//
    [!VAR "Index" = "0"!][!//
    [!/* strip end empty space */!][!//
    [!VAR "SlaveEventsGroup" = "normalize-space($SlaveEventsGroup)"!][!//
    [!/* check if class of slave events with their behavior was already found */!][!//
    [!IF "not(contains($MultiEventTriggeringSlaveGroupList, concat($SlaveEventsGroup, '#')))"!][!//
      [!VAR "MultiEventTriggeringClassList" = "concat($MultiEventTriggeringClassList, num:i($SlaveIdx), ' ', num:i($SlaveIdx + $SlaveCount), ' ', $Behavior, '#')"!][!//
      [!VAR "MultiEventTriggeringSlaveGroupList" = "concat($MultiEventTriggeringSlaveGroupList, $SlaveEventsGroup, '#')"!][!//
      [!VAR "SlaveIdx" = "$SlaveIdx + $SlaveCount"!][!//
      [!VAR "Index" = "$ClassIndex"!][!//
      [!VAR "ClassIndex" = "$ClassIndex + 1"!][!//
    [!ELSE!][!//
      [!LOOP "text:split($MultiEventTriggeringSlaveGroupList, '#')"!][!//
        [!IF ". != $SlaveEventsGroup"!][!//
          [!VAR "Index" = "$Index + 1"!][!//
        [!ELSE!][!//
          [!BREAK!][!//
        [!ENDIF!][!//
      [!ENDLOOP!][!//
    [!ENDIF!][!//
    [!VAR "MultiEventTriggeringClassIndexList" = "concat($MultiEventTriggeringClassIndexList, $MasterId, ':', num:i($Index), '#')"!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
* === Multi Event Triggering ===
* Returns the multi event triggering index of given EventId
* Macro CALC_MULTI_EVENT_CLASSES must be called before!
*/!][!//
[!MACRO "GET_MULTI_EVENT_TRIGGERING_INDEX", "EVENT_ID"!][!//
[!INDENT "0"!][!//
  [!VAR "MultiEventTriggeringClassIdx" = "$NumMultiEventTriggering"!][!//
  [!LOOP "text:split($MultiEventTriggeringClassIndexList, '#')"!][!//
    [!IF "text:split(.,':')[position() = 1] = num:i($EVENT_ID)"!][!//
      [!VAR "MultiEventTriggeringClassIdx" = "text:split(.,':')[position() = 2]"!][!//
      [!/* multi event trigggering configuration item found, stop searching */!][!//
      [!BREAK!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/*
* === Number of multi-event classes ===
* Macro CALC_MULTI_EVENT_CLASSES must be called before!
*/!][!//
[!MACRO "GET_MULTI_EVENT_TRIGGERING_CLASS_COUNT"!][!//
[!INDENT "0"!][!//
  [!VAR "NumMultiEventTriggeringClasses" = "count(text:split($MultiEventTriggeringClassList, '#'))"!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* 
* === Number of multi-event slave groups ===
* Macro CALC_MULTI_EVENT_CLASSES must be called before!
*/!][!//
[!MACRO "GET_MULTI_EVENT_TRIGGERING_SLAVE_COUNT"!][!//
[!INDENT "0"!][!//
  [!VAR "NumMultiEventTriggeringSlaves" = "num:i($SlaveIdx)"!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!//
[!/* 
* === Calculate number of NvM block Ids ===
* Macro GET_NVM_BLOCK_ID calculates the block id for configured NvM blocks!
*/!][!//
[!MACRO "GET_NVM_BLOCK_ID"!][!//
[!INDENT "0"!][!//
  [!VAR "DemNvRamBlockId" = "name(.)"!]
  [!VAR "DemNvRamBlockIdRefName" = "name(as:ref(DemNvRamBlockIdRef))"!]
  #if (defined [!"$DemNvRamBlockId"!])
  #error [!"$DemNvRamBlockId"!] already defined
  #endif
  /** \brief NvM block Id for block [!"$DemNvRamBlockId"!] */
  [!/* Naming shema according AUTOSAR R4.0 Rev 3 and newer*/!][!//
  #if defined (NvMConf_NvMBlockDescriptor_[!"$DemNvRamBlockIdRefName"!])
  #define [!"$DemNvRamBlockId"!] NvMConf_NvMBlockDescriptor_[!"$DemNvRamBlockIdRefName"!]
  [!/* Naming shema according AUTOSAR R3.1 Rev 4 till  R4.0 Rev 2*/!][!//
  #elif defined (NvM_[!"$DemNvRamBlockIdRefName"!])
  #define [!"$DemNvRamBlockId"!] NvM_[!"$DemNvRamBlockIdRefName"!]
  [!/* Naming shema according AUTOSAR R3.1 Rev 3 */!][!//
  #elif defined ([!"$DemNvRamBlockIdRefName"!])
  #define [!"$DemNvRamBlockId"!] [!"$DemNvRamBlockIdRefName"!]
  #else
  #error No symbolic name for NvM block [!"$DemNvRamBlockIdRefName"!] (refered by [!"$DemNvRamBlockId"!]) is generated
  #endif
[!ENDINDENT!][!//
[!ENDMACRO!][!//