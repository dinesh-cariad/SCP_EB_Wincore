/**
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
 */
#ifndef FIM_CFG_H
#define FIM_CFG_H

/* This file contains the generated FiM module configuration. */

[!AUTOSPACING!]
[!INCLUDE "FiM_Include.m"!]
[!//
[!/* determine FiM extension support and calibration support */!][!//
[!IF "node:exists(FiMGeneral/FiMExtendedSupport) and (FiMGeneral/FiMExtendedSupport = 'true')"!]
  [!VAR "ExtendedLevel" = "FiMGeneral/FiMExtendedLevel"!]
  [!VAR "FixedData" = "'false'"!]
[!ELSE!][!//
  [!VAR "ExtendedLevel" = "'FIM_NO_EXTENSION'"!]
  [!IF "node:exists(FiMGeneral/FiMDataFixed)"!]
    [!VAR "FixedData" = "FiMGeneral/FiMDataFixed"!]
  [!ELSE!]
    [!VAR "FixedData" = "'true'"!]
  [!ENDIF!]
[!ENDIF!]
[!//
[!VAR "Spaces31" = "'                               '"!]
[!//
[!/* verify all FIDs not using more than 255 Dem events */!][!//
[!/* this relates to FiM_FidConfig[].numEventCfg and to FiM_FidInhibitCounter[] */!][!//
[!IF "$ExtendedLevel != 'FIM_NO_EXTENSION'"!]
  [!VAR "MaxHandledFidEventLinksPerScheduling" = "FiMGeneral/FiMMaxHandledFidEventLinksPerScheduling"!]
[!ENDIF!]
[!LOOP "FiMConfigSet/*/FiMFID/*"!]
  [!/* count single Dem events of this FID */!][!//
  [!VAR "EventPerFidCount" = "count(../../FiMInhibitionConfiguration/*[text:split(FiMInhFunctionIdRef, '/')[last()] = name(node:current()) and FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'])"!]
  [!/* count individual Dem events of all event summaries of this FID */!][!//
  [!LOOP "../../FiMInhibitionConfiguration/*[text:split(FiMInhFunctionIdRef, '/')[last()] = name(node:current()) and FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceSumRef']"!]
    [!VAR "EventPerFidCount" = "$EventPerFidCount + count(../../FiMEventSummary/*[FiMOutputSumEventRef = node:current()/FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1]])"!]
  [!ENDLOOP!]
  [!IF "$EventPerFidCount > 255"!]
    [!ERROR!]
      More than 255 events (single Dem events and individual Dem events from event summaries) are assigned to the FID '[!"name(.)"!]'. Please reduce the number of assigned Dem events.
    [!ENDERROR!]
  [!ENDIF!]
  [!IF "$ExtendedLevel != 'FIM_NO_EXTENSION'"!]
    [!/* !LINKSTO dsn.FIM.VCC.Param.MaxHandledFidEventLinksPerSchedule.MinimumSize,1 */!][!//
    [!/* Part of this check is already done with XDM tests, but summary events are not included. */!][!//
    [!IF "$EventPerFidCount > $MaxHandledFidEventLinksPerScheduling"!]
      [!ERROR!]
        More than [!"$MaxHandledFidEventLinksPerScheduling"!] events (single Dem events and individual Dem events from event summaries) are assigned to the FID '[!"name(.)"!]'. Please reduce the number of assigned Dem events or increase the number of handled events per schedule, see 'FiMGeneral/FiMMaxHandledFidEventLinksPerScheduling'.
      [!ENDERROR!]
    [!ENDIF!]
    [!VAR "HighPrioEntry" = "0"!]
    [!LOOP "as:modconf('FiM')[1]/FiMConfigSet/*/FiMInhibitionConfiguration/*"!]
      [!IF "FiMHighPrioInhibition = 'true'"!]
        [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!]
          [!VAR "HighPrioEntry" = "$HighPrioEntry + 1"!]
        [!ELSEIF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceSumRef'"!][!//
          [!VAR "eventSumName" = "name(as:ref(FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1]))"!]
          [!LOOP "../../FiMEventSummary/*[text:split(FiMOutputSumEventRef, '/')[last()] = $eventSumName]"!]
            [!VAR "HighPrioEntry" = "$HighPrioEntry + 1"!]
          [!ENDLOOP!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDLOOP!]
    [!IF "$HighPrioEntry > $MaxHandledFidEventLinksPerScheduling"!]
      [!ERROR!]
        [!"num:i($HighPrioEntry)"!] events (single Dem events and individual Dem events from event summaries) are marked as high priority but only [!"$MaxHandledFidEventLinksPerScheduling"!] are allowed. Please reduce the number of high priority events or increase the number of allowed high priority inhibitions, see 'FiMGeneral/FiMMaxHighPrioEventToFidLinks'.
      [!ENDERROR!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]
[!//
/*==================[includes]===============================================*/

#include <Std_Types.h>  /* AUTOSAR standard types */
#include <Dem_Types.h>

/*==================[macros]=================================================*/

#if (defined FIM_CFG_SIGNATURE)
#error FIM_CFG_SIGNATURE is already defined
#endif
/* !LINKSTO FIM.EB.Config.Id,1 */
/** \brief Sign a certain configuration with unique hash
 **/
#define FIM_CFG_SIGNATURE [!"asc:getConfigSignature(node:difference(as:modconf('FiM')[1]//
                            *[not(child::*) and(node:configclass() = 'PreCompile')],
                            as:modconf('FiM')[1]/FiMConfigSet/*))"!]U

/*------------------[API configuration options]------------------------------*/

/* !LINKSTO FIM033,1 */
#if (defined FIM_VERSION_INFO_API)
#error FIM_VERSION_INFO_API already defined
#endif
/** \brief Switch, indicating if Version Info is activated for FiM */
[!IF "FiMGeneral/FiMVersionInfoApi = 'true'"!]
  [!WS "0"!]#define FIM_VERSION_INFO_API         STD_ON
[!ELSE!][!//
  [!WS "0"!]#define FIM_VERSION_INFO_API         STD_OFF
[!ENDIF!]

#if (defined FIM_DEV_ERROR_DETECT)
#error FIM_DEV_ERROR_DETECT already defined
#endif
/** \brief Switch, indicating if the detection of development errors is
 ** activated or deactivated for FiM */
[!IF "FiMGeneral/FiMDevErrorDetect = 'true'"!]
  [!WS "0"!]#define FIM_DEV_ERROR_DETECT         STD_ON
[!ELSE!][!//
  [!WS "0"!]#define FIM_DEV_ERROR_DETECT         STD_OFF
[!ENDIF!]

#if (defined FIM_EVENT_UPDATE_TRIGGERED_BY_DEM)
#error FIM_EVENT_UPDATE_TRIGGERED_BY_DEM already defined
#endif
/** \brief Switch, indicating if the Event update triggered by Dem is activated or
 ** deactivated for FiM */
[!IF "FiMGeneral/FiMEventUpdateTriggeredByDem = 'true'"!]
  [!WS "0"!]#define FIM_EVENT_UPDATE_TRIGGERED_BY_DEM   STD_ON
[!ELSE!][!//
  [!WS "0"!]#define FIM_EVENT_UPDATE_TRIGGERED_BY_DEM   STD_OFF
[!ENDIF!]

#if (defined FIM_DATA_FIXED)
#error FIM_DATA_FIXED already defined
#endif
/* !LINKSTO dsn.FIM.Param.CalibrationSupport,1 */
/** \brief Switch, indicating if the data is fixed */
[!IF "$FixedData = 'true'"!]
  [!WS "0"!]#define FIM_DATA_FIXED      STD_ON
[!ELSE!][!//
  [!WS "0"!]#define FIM_DATA_FIXED      STD_OFF
[!ENDIF!]

#if (defined FIM_INCLUDE_RTE)
#error FIM_INCLUDE_RTE already defined
#endif
/** \brief Switch, indicating if RTE is activated or deactivated for FiM */
[!IF "FiMGeneral/FiMRteUsage = 'true'"!]
  [!WS "0"!]#define FIM_INCLUDE_RTE              STD_ON
[!ELSE!][!//
  [!WS "0"!]#define FIM_INCLUDE_RTE              STD_OFF
[!ENDIF!]

#if (defined FIM_MEASURMENT_SUPPORT)
#error FIM_MEASURMENT_SUPPORT already defined
#endif
/** \brief Switch, indicating if Measurement Support is activated for FiM */
[!IF "FiMGeneral/FiMMeasurementSupport = 'true'"!]
  [!WS "0"!]#define FIM_MEASURMENT_SUPPORT         STD_ON
[!ELSE!][!//
  [!WS "0"!]#define FIM_MEASURMENT_SUPPORT         STD_OFF
[!ENDIF!]

[!INDENT "0"!]
  #if (defined FIM_EXTENDED_LEVEL)
  #error FIM_EXTENDED_LEVEL already defined
  #endif
  /* !LINKSTO dsn.FIM.VCC.Param.ExtendedLevel,1 */
  /** \brief Extended level can be FIM_NO_EXTENSION, FIM_EXTENSION_LVL1 or
             [!WS "11"!]FIM_EXTENSION_LVL2 */
  #define FIM_EXTENDED_LEVEL            [!"$ExtendedLevel"!]

  #if (defined FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS)
  #error FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS already defined
  #endif
  /* !LINKSTO dsn.FIM.VCC.Param.MaxHighPrioEventToFidLinks,1 */
  /** \brief Defines the maximal priority for event to FID links */
  [!IF "$ExtendedLevel = 'FIM_NO_EXTENSION'"!]
    #define FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS            0U
  [!ELSE!][!//
    #define FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS            [!"FiMGeneral/FiMMaxHighPrioEventToFidLinks"!]U
  [!ENDIF!]

  #if (defined FIM_EXT_MAX_HANDLED_FID_EVENT_LINKS_PER_SCHEDULING)
  #error FIM_EXT_MAX_HANDLED_FID_EVENT_LINKS_PER_SCHEDULING already defined
  #endif
  /* !LINKSTO dsn.FIM.VCC.Param.MaxHandledFidEventLinksPerSchedule,1, dsn.FiM.VCC.Param.MaxHandledFidEventLinksPerSchedule.Value,1 */
  /** \brief Defines the maximum FID event links handled per scheduling. */
  [!IF "$ExtendedLevel = 'FIM_NO_EXTENSION'"!]
    #define FIM_EXT_MAX_HANDLED_FID_EVENT_LINKS_PER_SCHEDULING  0U
  [!ELSE!][!//
    #define FIM_EXT_MAX_HANDLED_FID_EVENT_LINKS_PER_SCHEDULING  [!"FiMGeneral/FiMMaxHandledFidEventLinksPerScheduling"!]U
  [!ENDIF!]


  #if (defined FIM_MODE_POLLING_SYNC)
  #error FIM_MODE_POLLING_SYNC already defined
  #endif
  /** \brief Define for synchronous polling mode  */
  #define FIM_MODE_POLLING_SYNC       0U

  #if (defined FIM_MODE_POLLING_ASYNC)
  #error FIM_MODE_POLLING_ASYNC already defined
  #endif
  /** \brief Define for asynchronous polling mode  */
  #define FIM_MODE_POLLING_ASYNC      1U

  #if (defined FIM_MODE_TRIGGERED)
  #error FIM_MODE_TRIGGERED already defined
  #endif
  /** \brief Define for triggerd mode  */
  #define FIM_MODE_TRIGGERED          2U

  #if (defined FIM_MODE)
  #error FIM_MODE already defined
  #endif
  /** !LINKSTO dsn.FiM.FidCalculation.Mode,1 */
  /** \brief Defines the mode how FIDs are calculated. */
  [!IF "$ExtendedLevel = 'FIM_NO_EXTENSION'"!]
    [!IF "FiMGeneral/FiMEventUpdateTriggeredByDem = 'false'"!]
      #define FIM_MODE    FIM_MODE_POLLING_SYNC
    [!ELSE!][!//
      #define FIM_MODE    FIM_MODE_TRIGGERED
    [!ENDIF!]
  [!ELSE!][!//
    #define FIM_MODE      FIM_MODE_POLLING_ASYNC
  [!ENDIF!]
[!ENDINDENT!]

#if (defined FIM_MAXIMUM_EVENT_FID_LINKS)
#error FIM_MAXIMUM_EVENT_FID_LINKS already defined
#endif
/** \brief This configuration parameter specifies the total maximum number of
 ** links between EventIds and FIDs. */
#define FIM_MAXIMUM_EVENT_FID_LINKS  [!"num:integer(FiMGeneral/FiMMaxEventFidLinks)"!]U

#if (defined FIM_MAXIMUM_FIDS_PER_EVENT)
#error FIM_MAXIMUM_FIDS_PER_EVENT already defined
#endif
/** \brief This configuration parameter specifies the maximum number of FIDs
 ** that can be linked to a single event. */
#define FIM_MAXIMUM_FIDS_PER_EVENT   [!"num:integer(FiMGeneral/FiMMaxFidsPerEvent)"!]U

#if (defined FIM_MAXIMUM_EVENTS_PER_FID)
#error FIM_MAXIMUM_EVENTS_PER_FID already defined
#endif
/** \brief This configuration parameter specifies the maximum number of
 ** EventIds that can be linked to a single FID. */
#define FIM_MAXIMUM_EVENTS_PER_FID   [!"num:integer(FiMGeneral/FiMMaxEventsPerFid)"!]U

#if (defined FIM_MAXIMUM_SUMMARY_EVENTS)
#error FIM_MAXIMUM_SUMMARY_EVENTS already defined
#endif
/** \brief This configuration parameter specifies the maximum number of
 ** summarized events that can be configured. */
#define FIM_MAXIMUM_SUMMARY_EVENTS   [!"num:integer(FiMGeneral/FiMMaxSummaryEvents)"!]U

#if (defined FIM_MAXIMUM_SUMMARY_LINKS)
#error FIM_MAXIMUM_SUMMARY_LINKS already defined
#endif
/** \brief This configuration parameter specifies the total maximum number of
 ** links between EventIds and summarized events. */
#define FIM_MAXIMUM_SUMMARY_LINKS    [!"num:integer(FiMGeneral/FiMMaxSummaryLinks)"!]U

#if (defined FIM_MAXIMUM_TOTAL_LINKS)
#error FIM_MAXIMUM_TOTAL_LINKS already defined
#endif
/** \brief This configuration parameter specifies the total maximum number of
 ** links between EventIds and FIDs plus the number of links between EventIds
 ** and summarized events. */
#define FIM_MAXIMUM_TOTAL_LINKS      [!"num:integer(FiMGeneral/FiMMaxTotalLinks)"!]U

/*------------------[FIDs configuration]-------------------------------------*/

#if (defined FIM_FID_NUM)
#error FIM_FID_NUM already defined
#endif
/** \brief This configuration parameter specifies the total number of FIDs.
 **
 ** Calculated by number of configured FIDs including the invalid FID 0. */
#define FIM_FID_NUM    [!"num:i(count(FiMConfigSet/*/FiMFID/*) + 1)"!]U

[!INDENT "0"!]
  [!VAR "ConfigDataNum" = "0"!]
  [!VAR "ConfigStartIndex" = "1"!]
  [!IF "FiMGeneral/FiMEventUpdateTriggeredByDem = 'true' and $FixedData = 'true'"!]
    [!//
    [!CALL "GET_FIM_INHIBITION_CONFIGURATION_LISTS"!][!//
    [!/* loop through the sorted event list and parse each entry with respect to type of event - FID link */!][!//
    [!LOOP "text:split($FiMEventInhList, ';')"!]
      [!VAR "FIDCount" = "0"!]
      [!VAR "eventName" = "text:split(., ',')[1]"!]
      [!/* Get the FIDs directly linked to the selected event, if there are any */!][!//
      [!LOOP "text:split($FiMDemChoiceList, ';')"!]
        [!IF "text:split(., ',')[1] = $eventName"!]
          [!/* FID-link to Dem Event found */!][!//
          [!VAR "FIDCount" = "$FIDCount + 1"!]
        [!ENDIF!]
      [!ENDLOOP!]
      [!/* check if the event is linked via summaries */!][!//
      [!IF "not(text:split(., ',')[1] = text:split(., ',')[last()])"!]
        [!LOOP "text:split(., ',')"!]
          [!VAR "EventSummaryName" = "."!]
          [!LOOP "text:split($FiMSummaryList, ';')"!]
            [!IF "text:split(.,',')[1] = $EventSummaryName"!]
              [!/* FID-link via event summary found */!][!//
              [!VAR "FIDCount" = "$FIDCount + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      [!ENDIF!]
      [!IF "num:i($FIDCount) > 0"!]
        [!VAR "ConfigStartIndex" = "$ConfigStartIndex + 1"!]
        [!VAR "ConfigDataNum" = "$ConfigDataNum + $FIDCount"!]
      [!ENDIF!]
    [!ENDLOOP!]
    #if (defined FIM_CONFIG_ACCESS)
    #error FIM_CONFIG_ACCESS already defined
    #endif
    /** \brief This configuration parameter specifies the order of config data. */
    #define FIM_CONFIG_ACCESS    FIM_CONFIG_ACCESS_EVENT

    #if (defined FIM_EVT_STARTINDEX_NUM)
    #error FIM_EVT_STARTINDEX_NUM already defined
    #endif
    /** \brief This configuration parameter specifies the total number of FIDs.
     **
     ** Calculated by number of configured FIDs including the invalid FID 0. */
    #define FIM_EVT_STARTINDEX_NUM    [!"num:i($ConfigStartIndex)"!]U

    #if (defined FIM_EVT_CONFIGDATA_NUM)
    #error FIM_EVT_CONFIGDATA_NUM already defined
    #endif
    /** \brief This configuration parameter specifies the total number of FIDs.
     **
     ** Calculated by number of configured FIDs including the invalid FID 0. */
    #define FIM_EVT_CONFIGDATA_NUM    [!"num:i($ConfigDataNum)"!]U
  [!ELSE!]
    [!VAR "ConfigStartIndex" = "num:i(count(FiMConfigSet/*/FiMFID/*) + 1)"!]
    [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
      [!LOOP "../../FiMInhibitionConfiguration/*[text:split(FiMInhFunctionIdRef, '/')[last()] = name(node:current())]"!]
        [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!]
         [!VAR "ConfigDataNum" = "$ConfigDataNum + 1"!]
        [!ELSE!][!//
          [!LOOP "../../FiMEventSummary/*[FiMOutputSumEventRef = node:current()/FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1]]"!]
            [!VAR "ConfigDataNum" = "$ConfigDataNum + 1"!]
          [!ENDLOOP!]
        [!ENDIF!]
      [!ENDLOOP!]
    [!ENDLOOP!]
    #if (defined FIM_CONFIG_ACCESS)
    #error FIM_CONFIG_ACCESS already defined
    #endif
    /** \brief This configuration parameter specifies the order of config data. */
    #define FIM_CONFIG_ACCESS    FIM_CONFIG_ACCESS_FID

    #if (defined FIM_FID_STARTINDEX_NUM)
    #error FIM_FID_STARTINDEX_NUM already defined
    #endif
    /** \brief This configuration parameter specifies the total number of start indexes of FIDs.
     **
     ** Calculated by number of configured FIDs including the invalid FID 0. */
    #define FIM_FID_STARTINDEX_NUM    [!"num:i($ConfigStartIndex)"!]U

    #if (defined FIM_FID_CONFIGDATA_NUM)
    #error FIM_FID_CONFIGDATA_NUM already defined
    #endif
    /** \brief This configuration parameter specifies the total number of FIDs.
     **
     ** Calculated by number of configured FIDs including the invalid FID 0. */
    #define FIM_FID_CONFIGDATA_NUM    [!"num:i($ConfigDataNum)"!]U

    #if (defined FIM_EXT_CONFIG_NUM)
    #error FIM_EXT_CONFIG_NUM already defined
    #endif
    /** \brief This configuration parameter specifies the total number of Messages.
     **
     ** Calculated by number of configured FIDs. */
    #define FIM_EXT_CONFIG_NUM        [!"num:i(count(FiMConfigSet/*/FiMFID/*))"!]U

    #if (defined FIM_MESSAGE_MAX)
    #error FIM_MESSAGE_MAX already defined
    #endif
    /** \brief Maximal number of messages */
    #define FIM_MESSAGE_MAX   [!"num:i(count(FiMConfigSet/*/FiMMessage/*) + 1)"!]U
  [!ENDIF!]
[!ENDINDENT!]

/* Symbolic names of configured Function IDs */
[!INDENT "0"!]
  [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
    [!VAR "Indention" = "''"!]
    [!IF "string-length($Spaces31) > string-length(name(.))"!]
      [!VAR "Indention" = "substring($Spaces31, string-length(name(.)))"!]
    [!ENDIF!]

    #if (defined FiMConf_FiMFID_[!"name(.)"!])
    #error FiMConf_FiMFID_[!"name(.)"!] already defined
    #endif
    /** \brief Export symbolic name value */
    #define FiMConf_FiMFID_[!"name(.)"!][!"$Indention"!] [!"FiMFunctionId"!]U

    #ifndef FIM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
    #if (defined [!"name(.)"!])
    #error [!"name(.)"!] already defined
    #endif
    /** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
    #define [!"name(.)"!]               [!"$Indention"!] [!"FiMFunctionId"!]U

    #if (defined FiM_[!"name(.)"!])
    #error FiM_[!"name(.)"!] already defined
    #endif
    /** \brief Export symbolic name value with module abbreviation as prefix
     [!WS!]** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
    #define FiM_[!"name(.)"!]           [!"$Indention"!] [!"FiMFunctionId"!]U
    #endif /* ifndef FIM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
  [!ENDLOOP!]

  #if (defined FIM_MAX_DEM_EVENT_ID)
  #error FIM_MAX_DEM_EVENT_ID already defined
  #endif
  /** \brief This configuration parameter specifies the largest event Id
   [!WS!]** configured in Dem. */
  #define FIM_MAX_DEM_EVENT_ID  [!"num:max(as:modconf('Dem')[1]/DemConfigSet/*/DemEventParameter/*/DemEventId)"!]U
[!ENDINDENT!]

/*------------------[Event summaries configuration]----------------------------*/

/* Symbolic names of configured Event Summary IDs - not relevant to this implementation */
[!INDENT "0"!]
  [!LOOP "node:order(FiMConfigSet/*/FiMSummaryEventId/*, 'FiMEventSumId')"!]
    [!VAR "Indention" = "''"!]
    [!IF "string-length($Spaces31) > string-length(name(.))"!]
      [!VAR "Indention" = "substring($Spaces31, string-length(name(.)))"!]
    [!ENDIF!]

    #if (defined FiMConf_FiMSummaryEventId_[!"name(.)"!])
    #error FiMConf_FiMSummaryEventId_[!"name(.)"!] already defined
    #endif
    /** \brief Export symbolic name value */
    #define FiMConf_FiMSummaryEventId_[!"name(.)"!][!"$Indention"!] [!"FiMEventSumId"!]U

    #ifndef FIM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES
    #if (defined [!"name(.)"!])
    #error [!"name(.)"!] already defined
    #endif
    /** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
    #define [!"name(.)"!]                          [!"$Indention"!] [!"FiMEventSumId"!]U

    #if (defined FiM_[!"name(.)"!])
    #error FiM_[!"name(.)"!] already defined
    #endif
    /** \brief Export symbolic name value with module abbreviation as prefix
     [!WS!]** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
    #define FiM_[!"name(.)"!]                      [!"$Indention"!] [!"FiMEventSumId"!]U
    #endif /* ifndef FIM_DONT_PROVIDE_LEGACY_SYMBOLIC_NAMES */
  [!ENDLOOP!]
[!ENDINDENT!]


#if (defined FIM_FID_INVALID)
#error FIM_FID_INVALID already defined
#endif
/** \brief Invalid event symbolic name value */
#define FIM_FID_INVALID       0U

#if (defined FIM_EVENT_INVALID)
#error FIM_EVENT_INVALID already defined
#endif
/** \brief Invalid event symbolic name value */
#define FIM_EVENT_INVALID     0U

#if (defined FIM_MESSAGE_INVALID)
#error FIM_MESSAGE_INVALID already defined
#endif
/* !LINKSTO VCC-FIM-003:Req38v1,1 */
/** \brief Invalid message symbolic name value */
#define FIM_MESSAGE_INVALID   0U

/*==================[type definitions]=======================================*/

/*------------------[FiM_ConfigType]----------------------------------------*/

/** \brief This type defines a data structure for the post build
 ** parameters of the FiM
 **
 ** \note Type is unused, as only pre-compile time support is implemented. */
typedef uint8 FiM_ConfigType;

[!INDENT "0"!]
  /** \brief Type of start index */
  [!IF "$ConfigDataNum > 255"!]
    typedef uint16 FiM_IndexType;
  [!ELSE!]
    typedef uint8 FiM_IndexType;
  [!ENDIF!]
[!ENDINDENT!]

[!IF "FiMGeneral/FiMEventUpdateTriggeredByDem = 'true' and $FixedData = 'true'"!]
/*--------------[ FiM 'triggered mode' ]------------------------------------*/
/* !LINKSTO dsn.FiM.StaticData,1 */

  [!INDENT "0"!]
    /** \brief Start index structure for FiM 'triggered mode' */
    typedef struct
    {
      [!INDENT "2"!]
        /** \brief Event ID */
        Dem_EventIdType EventID;
        /** \brief Number of associated FIDs */
        FiM_IndexType   StartIndex;
      [!ENDINDENT!]
    } FiM_EvtConfigType;

    /** \brief Config data structure for FiM 'triggered mode' */
    typedef uint16 FiM_EvtConfigDataType;
  [!ENDINDENT!]

[!ELSE!]
/*--------------[ FiM 'polling mode' or 'calibration support' ]---------------*/
/* !LINKSTO dsn.FiM.StaticData,1 */

  [!INDENT "0"!]
    /** \brief Start index structure for FiM 'polling mode' or calibration mode */
    typedef FiM_IndexType FiM_FidConfigStartIndexType;

    /** \brief Config data structure for FiM 'polling mode' or calibration mode */
    typedef uint16 FiM_FidConfigDataType;
  [!ENDINDENT!]

[!ENDIF!]
/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#define FIM_START_SEC_CONST_UNSPECIFIED
#include <FiM_MemMap.h>

/** \brief Configuration structure */
extern CONST(FiM_ConfigType, FIM_CONST) [!"name(FiMConfigSet/*[1])"!];

#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include <FiM_MemMap.h>

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FIM_CFG_H */
/*==================[end of file]============================================*/
