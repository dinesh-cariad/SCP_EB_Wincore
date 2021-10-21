[!/**
 * \file
 *
 * \brief AUTOSAR FiM
 *
 * This file contains the implementation of the AUTOSAR
 * module FiM.
 *
 * \version 2.5.3
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!//
[!/*
 * === Get FiMEventInhList ===
 * This Macro will fill the variable FiMEventInhList
 * with a string of the following structure:
 *
 * DemEventParameter 
 * FimSummaryEventId: List of varible number of parameters separated by ","
 *
 * The string list is ordered by EventId and contains
 * events that are directly linked to FIDs and events
 * that are linked with FIDs via EventSummmary parameters.
 *
 * Each entry in the list is separated by ";".
 * NOTE: An entry contains only the event name if the event is linked directly - via FiMInhChoiceDemRef
 */!][!//
[!MACRO "GET_FIM_EVENT_INH_LIST"!][!//
[!INDENT "0"!][!//
  [!VAR "FiMEventInhList" = "''"!][!//
  [!/* loop over DemEvent containers and get the ones having links to FiMEventSummary containers */!][!//
  [!LOOP "node:order(as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*, 'DemEventId')"!][!//
    [!VAR "eventName" = "name(.)"!][!//
    [!VAR "InhChoiceFlag" = "'false'"!][!//
    [!/* Look through the inhibition configuration items and grab directly linked events to fids */!][!//
    [!IF "node:exists(as:modconf('FiM')[1]/FiMConfigSet/*/FiMInhibitionConfiguration/*[FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef' and text:split(FiMInhEventId/FiMInhRefChoice/FiMInhEventRef, '/')[last()] = $eventName])"!][!//
      [!VAR "FiMEventInhList" = "concat($FiMEventInhList, $eventName)"!][!//
      [!VAR "InhChoiceFlag" = "'true'"!][!//
    [!ENDIF!][!//
    [!/* Look through the summary link configuration items and grab events linked to summaries to fids */!][!//
    [!LOOP "as:modconf('FiM')[1]/FiMConfigSet/*/FiMEventSummary/*[text:split(FiMInputSumEventRef/*[1], '/')[last()] = $eventName]"!][!//
      [!IF "$InhChoiceFlag = 'false'"!][!//
        [!VAR "FiMEventInhList" = "concat($FiMEventInhList, $eventName)"!][!//
        [!VAR "InhChoiceFlag" = "'true'"!][!//
      [!ENDIF!][!//
      [!VAR "FiMEventInhList" = "concat($FiMEventInhList, ',', text:split(FiMOutputSumEventRef, '/')[last()])"!][!//
    [!ENDLOOP!][!//
    [!IF "$InhChoiceFlag = 'true'"!][!//
      [!VAR "FiMEventInhList" = "concat($FiMEventInhList, ';')"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//
[!/*
 * === Get FiMInhibitionConfigurationLists ===
 * This Macro will fill the variables FiMSummaryList and FiMDemChoiceList
 * with a string of the following structure:
 *
 * FiMDemChoiceList:
 * DemEventParameter
 * FiMInhInhibitionMask
 * FiMInhFunctionId
 * 
 * The string list is not ordered and each entry elements are separated by ","
 * Each entry in the list is separated by ";".
 * An entry contains events directly linked to FIDs via FiMInhChoiceDemRef/FiMInhEventRef
 *
 * FiMSummaryList:
 * FiMInhSum
 * FiMInhInhibitionMask
 * FiMInhFunctionId
 *
 * The string list is not ordered and each entry elements are separated by ","
 * Each entry in the list is separated by ";".
 * An entry contains events directly linked to FIDs via FiMInhChoiceDemRef/FiMInhSumRef
 */!][!//
[!MACRO "GET_FIM_INHIBITION_CONFIGURATION_LISTS"!][!//
[!INDENT "0"!][!//
  [!CALL "GET_FIM_EVENT_INH_LIST"!][!//
  [!VAR "FiMSummaryList" = "''"!][!//
  [!VAR "FiMDemChoiceList" = "''"!][!//
  [!/* loop over all Dem events and get for each one their associated summary */!][!//
  [!LOOP "as:modconf('FiM')[1]/FiMConfigSet/*/FiMInhibitionConfiguration/*"!][!//
    [!VAR "InhInhibitionMask" = "FiMInhInhibitionMask"!][!//
    [!VAR "FIDName" = "text:split(FiMInhFunctionIdRef, '/')[last()]"!][!//
    [!VAR "MaskAndFid" = "concat($InhInhibitionMask, ',', $FIDName)"!][!//
    [!/* single FID associated to Dem event */!][!//
    [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!][!//
      [!VAR "eventName" = "text:split(FiMInhEventId/FiMInhRefChoice/FiMInhEventRef, '/')[last()]"!][!//
      [!VAR "FiMDemChoiceList" = "concat($FiMDemChoiceList, $eventName, ',', $MaskAndFid, ';')"!][!//
    [!ELSE!][!//
      [!VAR "eventSumName" = "text:split(FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1], '/')[last()]"!][!//
      [!VAR "FiMSummaryList" = "concat($FiMSummaryList, $eventSumName, ',', $MaskAndFid, ';')"!][!//
    [!ENDIF!][!//
  [!ENDLOOP!][!//
[!ENDINDENT!][!//
[!ENDMACRO!][!//