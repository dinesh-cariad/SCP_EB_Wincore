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

[!AUTOSPACING!]
[!INCLUDE "../include/FiM_Include.m"!]
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
/*==================[inclusions]============================================*/
/* !LINKSTO dsn.FiM.IncludeStr,1 */

#include <Std_Types.h>         /* AUTOSAR standard types */

#include <Dem.h>               /* Dem event IDs */

#include <FiM.h>               /* Module public API */
#include <FiM_Int.h>           /* Module internal interface */
[!IF "$ExtendedLevel != 'FIM_NO_EXTENSION'"!]
#include <FiM_Message.h>
[!ENDIF!]

/*==================[macros]================================================*/

/* bit masks for the inhibition mask configuration part of the event link
 * configuration */

#if (defined FIM_LAST_FAILED_CFG_MASK)
#error FIM_LAST_FAILED_CFG_MASK already defined
#endif
/** \brief FiM last failed inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_LAST_FAILED_CFG_MASK                ((uint16)0U << FIM_CONFIG_INHIBITION_MASK_OFFSET)

#if (defined FIM_NOT_TESTED_CFG_MASK)
#error FIM_NOT_TESTED_CFG_MASK already defined
#endif
/** \brief FiM not tested inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_NOT_TESTED_CFG_MASK                 ((uint16)1U << FIM_CONFIG_INHIBITION_MASK_OFFSET)

#if (defined FIM_TESTED_CFG_MASK)
#error FIM_TESTED_CFG_MASK already defined
#endif
/** \brief FiM tested inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_TESTED_CFG_MASK                     ((uint16)2U << FIM_CONFIG_INHIBITION_MASK_OFFSET)

#if (defined FIM_TESTED_AND_FAILED_CFG_MASK)
#error FIM_TESTED_AND_FAILED_CFG_MASK already defined
#endif
/** \brief FiM tested and failed inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_TESTED_AND_FAILED_CFG_MASK          ((uint16)3U << FIM_CONFIG_INHIBITION_MASK_OFFSET)


#if (FIM_EXTENDED_LEVEL != FIM_NO_EXTENSION)

#if (defined FIM_FAILED_THIS_OP_CYCLE_CFG_MASK)
#error FIM_FAILED_THIS_OP_CYCLE_CFG_MASK already defined
#endif
/** \brief UDS status byte bit 1 (testFailedThisOperationCycle) inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_FAILED_THIS_OP_CYCLE_CFG_MASK       ((uint16)4U << FIM_CONFIG_INHIBITION_MASK_OFFSET)

#if (defined FIM_LAST_UNCONFIRMED_CFG_MASK)
#error FIM_LAST_UNCONFIRMED_CFG_MASK already defined
#endif
/** \brief SI30 status byte bit 0 (UnconfirmedDTC) inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_LAST_UNCONFIRMED_CFG_MASK           ((uint16)5U << FIM_CONFIG_INHIBITION_MASK_OFFSET)

#if (defined FIM_UNCONFIRMED_THIS_OP_CYCLE_CFG_MASK)
#error FIM_UNCONFIRMED_THIS_OP_CYCLE_CFG_MASK already defined
#endif
/** \brief SI30 status byte bit 1 (UnconfirmedDTCThisOperationCycle) inhibition mask index configuration mask
 * (used by FiM_EvtConfigData[] and FiM_FidConfigData[]) */
#define FIM_UNCONFIRMED_THIS_OP_CYCLE_CFG_MASK  ((uint16)6U << FIM_CONFIG_INHIBITION_MASK_OFFSET)

#endif

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[internal constants]====================================*/

/*==================[external constants]====================================*/

[!VAR "StartIndex" = "0"!]
[!VAR "StringFiM_EvtConfigElements" = "''"!]

#define FIM_START_SEC_CONST_UNSPECIFIED
#include <FiM_MemMap.h>

CONST(FiM_ConfigType, FIM_CONST) [!"name(FiMConfigSet/*[1])"!] = 0U;

#define FIM_STOP_SEC_CONST_UNSPECIFIED
#include <FiM_MemMap.h>

[!INDENT "0"!]
  [!IF "FiMGeneral/FiMEventUpdateTriggeredByDem = 'true' and $FixedData = 'true'"!]
    #define FIM_START_SEC_CALIB_16
    #include <FiM_MemMap.h>

    /** \brief Array of all calibrated inhibition mask and FID for Dem events */
    CONST(FiM_EvtConfigDataType, FIM_CONST) FiM_EvtConfigData[FIM_EVT_CONFIGDATA_NUM] =
    {
      [!INDENT "2"!]
        [!//
        [!CALL "GET_FIM_INHIBITION_CONFIGURATION_LISTS"!][!//
        [!/* loop through the sorted event list and parse each entry with respect to type of event - FID link */!][!//
        [!LOOP "text:split($FiMEventInhList, ';')"!]
          [!VAR "FIDCount" = "0"!]
          [!VAR "eventName" = "text:split(., ',')[1]"!]
          /* ----- FIDs associated with event '[!"$eventName"!]' ----- */
          [!/* Get the FIDs directly linked to the selected event, if there are any */!][!//
          [!LOOP "text:split($FiMDemChoiceList, ';')"!]
            [!IF "text:split(., ',')[1] = $eventName"!]
              /* FID-link to Dem Event */
              [!"text:split(., ',')[2]"!]_CFG_MASK | FiMConf_FiMFID_[!"text:split(., ',')[3]"!],
              [!VAR "FIDCount" = "$FIDCount + 1"!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!/* check if the event is linked via summaries */!][!//
          [!IF "not(text:split(., ',')[1] = text:split(., ',')[last()])"!]
            [!LOOP "text:split(., ',')"!]
              [!VAR "EventSummaryName" = "."!]
              [!LOOP "text:split($FiMSummaryList, ';')"!]
                [!IF "text:split(.,',')[1] = $EventSummaryName"!]
                  /* FID-link via event summary [!"$EventSummaryName"!] */
                  [!"text:split(., ',')[2]"!]_CFG_MASK | FiMConf_FiMFID_[!"text:split(., ',')[3]"!],
                  [!VAR "FIDCount" = "$FIDCount + 1"!]
                [!ENDIF!]
              [!ENDLOOP!]
            [!ENDLOOP!]
          [!ENDIF!]
          [!IF "num:i($FIDCount) > 0"!]
            [!VAR "StringFiM_EvtConfigElements"!][!"$StringFiM_EvtConfigElements"!][!"$eventName"!],[!"num:i($FIDCount)"!],[!"num:i($StartIndex)"!];[!ENDVAR!]
            [!VAR "StartIndex" = "$StartIndex + $FIDCount"!]
          [!ENDIF!]
        [!ENDLOOP!]
      [!ENDINDENT!]
    };

    /* !LINKSTO dsn.FIM.DataStructure.AccessedByEventId,1, dsn.FIM.DataStructure.TriggeredMode,1 */
    /** \brief Array of calibrated Dem events with start index of the
     [!WS!]** corresponding FIDs in the data array */
    CONST(FiM_EvtConfigType, FIM_CONST) FiM_EvtConfig[FIM_EVT_STARTINDEX_NUM] =
    {
      [!INDENT "2"!]
        [!LOOP "text:split($StringFiM_EvtConfigElements,';')"!]
          {
            [!INDENT "4"!]
            /* Event: [!"(text:split(.,',')[1])"!] associated by [!"num:i(text:split(.,',')[2])"!] FIDs */
            DemConf_DemEventParameter_[!"(text:split(.,',')[1])"!], [!"num:i(text:split(.,',')[3])"!]U
            [!ENDINDENT!]
          },
        [!ENDLOOP!]
        {
          [!INDENT "4"!]
            /* dummy entry to hold the maximum index number */
            FIM_EVENT_INVALID, [!"num:i($StartIndex)"!]U
          [!ENDINDENT!]
        }
      [!ENDINDENT!]
    };

    #define FIM_STOP_SEC_CALIB_16
    #include <FiM_MemMap.h>
  [!ELSE!][!//
    #define FIM_START_SEC_CONST_UNSPECIFIED
    #include <FiM_MemMap.h>

    /* !LINKSTO dsn.FIM.DataStructure.AccessedByFID,1, dsn.FIM.DataStructure.PollingMode,1 */
    /** \brief Array of start index of the corresponding Dem events in the data array */
    CONST(FiM_FidConfigStartIndexType, FIM_CONST) FiM_FidConfigStartIndex[FIM_FID_STARTINDEX_NUM] =
    {
      [!INDENT "2"!]
        0U,
        [!VAR "Index" = "0"!]
        [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
          [!LOOP "../../FiMInhibitionConfiguration/*[text:split(FiMInhFunctionIdRef, '/')[last()] = name(node:current())]"!]
            [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!]
              [!VAR "Index" = "$Index + 1"!]
            [!ELSE!][!//
              [!LOOP "../../FiMEventSummary/*[FiMOutputSumEventRef = node:current()/FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1]]"!]
                [!VAR "Index" = "$Index + 1"!]
              [!ENDLOOP!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!"num:i($Index)"!]U,
        [!ENDLOOP!]
      [!ENDINDENT!]
    };

    #define FIM_STOP_SEC_CONST_UNSPECIFIED
    #include <FiM_MemMap.h>

    #define FIM_START_SEC_CALIB_16
    #include <FiM_MemMap.h>

    /* !LINKSTO dsn.FIM.DataStructure.Calibration,1, dsn.FiM.VCC.HighPriorityInhibition,1 */
    /** \brief Array of all calibrated inhibition mask and Dem events for FIDs */
    CONST(FiM_FidConfigDataType, FIM_CONST) FiM_FidConfigData[FIM_FID_CONFIGDATA_NUM] =
    {
      [!INDENT "2"!]
        [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
          [!/* loop over all inhibition configurations referring to this FID */!][!//
          /* ----- Events associated with FID '[!"name(node:current())"!]' ----- */
          [!LOOP "../../FiMInhibitionConfiguration/*[text:split(FiMInhFunctionIdRef, '/')[last()] = name(node:current())]"!]
            [!VAR "InhInhibitionMask" = "FiMInhInhibitionMask"!]
            [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!]
              /* single Dem event link */
              [!"$InhInhibitionMask"!]_CFG_MASK | DemConf_DemEventParameter_[!"text:split(FiMInhEventId/FiMInhRefChoice/FiMInhEventRef, '/')[last()]"!],
            [!ELSE!][!//
              /* Dem event links of the event summary [!"name(as:ref(FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1]))"!] */
              [!/* loop over all Dem events of the event summary referring to this inhibition config */!][!//
              [!LOOP "../../FiMEventSummary/*[FiMOutputSumEventRef = node:current()/FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1]]"!]
                [!"$InhInhibitionMask"!]_CFG_MASK | DemConf_DemEventParameter_[!"text:split(FiMInputSumEventRef/*[1], '/')[last()]"!],
              [!ENDLOOP!]
            [!ENDIF!]
          [!ENDLOOP!]
        [!ENDLOOP!]
      [!ENDINDENT!]
    };

    #define FIM_STOP_SEC_CALIB_16
    #include <FiM_MemMap.h>

    [!IF "$ExtendedLevel != 'FIM_NO_EXTENSION'"!]
    #define FIM_START_SEC_CALIB_16
    #include <FiM_MemMap.h>

      /* !LINKSTO VCC-FIM-004:Req11v2,1 */
      /** \brief FiM extended configuration */
      CONST(FiM_FidExtendedCfgType, FIM_CONST) FiM_ExtendedConfiguration[FIM_EXT_CONFIG_NUM] =
      {
        [!INDENT "2"!]
          [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
            [!VAR "ConditionEvent" = "'FIM_EVENT_INVALID'"!]
            [!VAR "SymptomBit" = "'0'"!]
            [!IF "node:refexists(FiMConditionEventRef)"!]
              [!VAR "ConditionEvent" = "concat('DemConf_DemEventParameter_', text:split(FiMConditionEventRef, '/')[last()])"!]
            [!ENDIF!]
            [!IF "FiMSetSymptom = 'true'"!]
              [!VAR "SymptomBit" = "'1'"!]
            [!ENDIF!]
              ([!"$ConditionEvent"!] << FIM_EXT_CONFIG_CONDITION_EVENTID_OFFSET) | ((FiM_FidExtendedCfgType)[!"$SymptomBit"!]U << FIM_EXT_CONFIG_SYMPTOM_OFFSET),
          [!ENDLOOP!]
        [!ENDINDENT!]
      };


#if (FIM_EXTENDED_LEVEL == FIM_EXTENSION_LVL2)
      /** \brief Extended configuration */
      CONST(FiM_FidExtendedCfgLvl2Type, FIM_CONST) FiM_ExtendedConfigurationLvl2[FIM_EXT_CONFIG_NUM] =
      {
        [!INDENT "2"!]
          [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
            [!VAR "RecoveryEvent" = "'FIM_EVENT_INVALID'"!]
            [!VAR "ForceReleaseEvent" = "'FIM_EVENT_INVALID'"!]
            [!IF "node:refexists(FiMRecoveryEventRef)"!]
              [!VAR "RecoveryEvent" = "concat('DemConf_DemEventParameter_', text:split(FiMRecoveryEventRef, '/')[last()])"!]
            [!ENDIF!]
            [!IF "node:refexists(FiMForcedReleaseEventRef)"!]
              [!VAR "ForceReleaseEvent" = "concat('DemConf_DemEventParameter_', text:split(FiMForcedReleaseEventRef, '/')[last()])"!]
            [!ENDIF!]
            { [!"$RecoveryEvent"!], [!"$ForceReleaseEvent"!] },
          [!ENDLOOP!]
        [!ENDINDENT!]
      };
#endif

      /** \brief High Priority EventId */
      CONST(Dem_EventIdType, FIM_CONST) FiM_HighPrioEventId[FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS] =
      {
        [!VAR "HighPrioEntry" = "0"!]
        [!INDENT "2"!]
          [!LOOP "as:modconf('FiM')[1]/FiMConfigSet/*/FiMInhibitionConfiguration/*"!]
            [!IF "FiMHighPrioInhibition = 'true'"!]
              [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!]
                [!VAR "HighPrioEntry" = "$HighPrioEntry + 1"!]
                /* Single Dem Event */
                DemConf_DemEventParameter_[!"text:split(FiMInhEventId/FiMInhRefChoice/FiMInhEventRef, '/')[last()]"!],
              [!ELSEIF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceSumRef'"!][!//
                [!VAR "eventSumName" = "text:split(FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1], '/')[last()]"!]
                [!LOOP "../../FiMEventSummary/*[text:split(FiMOutputSumEventRef, '/')[last()] = $eventSumName]"!]
                  [!VAR "HighPrioEntry" = "$HighPrioEntry + 1"!]
                  /* Dem Event via event summary [!"$eventSumName"!] */
                  DemConf_DemEventParameter_[!"text:split(FiMInputSumEventRef/*[1], '/')[last()]"!],
                [!ENDLOOP!]
              [!ENDIF!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!FOR "Idx" = "$HighPrioEntry" TO "FiMGeneral/FiMMaxHighPrioEventToFidLinks - 1"!]
            FIM_EVENT_INVALID,
          [!ENDFOR!]
        [!ENDINDENT!]
      };

#if (FIM_FID_NUM <= 256)
    #define FIM_STOP_SEC_CALIB_16
    #include <FiM_MemMap.h>

    #define FIM_START_SEC_CALIB_8
    #include <FiM_MemMap.h>
#endif

      /* !LINKSTO dsn.FiM.VCC.HighPriorityInhibition,1 */
      /** \brief High Priority Fid */
      CONST(FiM_FunctionIdType, FIM_CONST) FiM_HighPrioFid[FIM_EXT_MAX_HIGH_PRIO_EVENT_TO_FID_LINKS] =
      {
        [!VAR "HighPrioEntry" = "0"!]
        [!INDENT "2"!]
          [!LOOP "as:modconf('FiM')[1]/FiMConfigSet/*/FiMInhibitionConfiguration/*"!]
            [!IF "FiMHighPrioInhibition = 'true'"!]
              [!VAR "FIDName" = "text:split(FiMInhFunctionIdRef, '/')[last()]"!]
              [!IF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceDemRef'"!]
                [!VAR "HighPrioEntry" = "$HighPrioEntry + 1"!]
                /* FID-link to Dem Event */
                FiMConf_FiMFID_[!"$FIDName"!],
              [!ELSEIF "FiMInhEventId/FiMInhRefChoice = 'FiMInhChoiceSumRef'"!][!//
                [!VAR "eventSumName" = "text:split(FiMInhEventId/FiMInhRefChoice/FiMInhSumRef/*[1], '/')[last()]"!]
                [!LOOP "../../FiMEventSummary/*[text:split(FiMOutputSumEventRef, '/')[last()] = $eventSumName]"!]
                  [!VAR "HighPrioEntry" = "$HighPrioEntry + 1"!]
                  /* FID-link via event summary [!"$eventSumName"!] */
                  FiMConf_FiMFID_[!"$FIDName"!],
                [!ENDLOOP!]
              [!ENDIF!]
            [!ENDIF!]
          [!ENDLOOP!]
          [!FOR "Idx" = "$HighPrioEntry" TO "FiMGeneral/FiMMaxHighPrioEventToFidLinks - 1"!]
            FIM_FID_INVALID,
          [!ENDFOR!]
        [!ENDINDENT!]
      };

#if (FIM_FID_NUM > 256)
    #define FIM_STOP_SEC_CALIB_16
    #include <FiM_MemMap.h>

    #define FIM_START_SEC_CALIB_8
    #include <FiM_MemMap.h>
#endif

      /* !LINKSTO dsn.FIM.DataStructure.ExtendedMessageConfiguration,1 */
      /** \brief FiM Message Id configuration */
      CONST(FiM_MessageIdType, FIM_CONST) FiM_MessageIdConfiguration[FIM_EXT_CONFIG_NUM] =
      {
        [!INDENT "2"!]
          [!LOOP "node:order(FiMConfigSet/*/FiMFID/*, 'FiMFunctionId')"!]
            [!VAR "Message" = "'FIM_MESSAGE_INVALID'"!]
            [!IF "node:refexists(FiMMessageRef)"!]
              [!VAR "Message" = "text:split(FiMMessageRef, '/')[last()]"!]
            [!ENDIF!]
            [!"$Message"!],
          [!ENDLOOP!]
        [!ENDINDENT!]
      };
    #define FIM_STOP_SEC_CALIB_8
    #include <FiM_MemMap.h>
    [!ENDIF!]
  [!ENDIF!]
[!ENDINDENT!]

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

/*==================[end of file]===========================================*/
