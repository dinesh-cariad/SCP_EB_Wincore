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

[!// Included by generated pre-compile time configuration file



[!IF "not(var:defined('WDGM_CFG_M'))"!]
[!VAR "WDGM_CFG_M"="'true'"!]

[!// EXPORTED VARIABLES (boolean parameters) -------------------------------------------------------

[!VAR "BswCompatibilityMode" = "node:value(WdgMGeneral/WdgMBSWCompatibilityMode)"!]
[!VAR "DevErrorDetectEnabled" = "WdgMGeneral/WdgMDevErrorDetect = 'true'"!]
[!VAR "VersionInfoApiEnabled" = "WdgMGeneral/WdgMVersionInfoApi = 'true'"!]
[!VAR "DemStoppedSupervisionReportEnabled" = "WdgMGeneral/WdgMDemStoppedSupervisionReport = 'true'"!]
[!VAR "ImmediateResetEnabled" = "WdgMGeneral/WdgMImmediateReset = 'true'"!]
[!VAR "OffModeEnabled" = "WdgMGeneral/WdgMOffModeEnabled = 'true'"!]
[!VAR "RteUsageEnabled" = "WdgMGeneral/WdgMRteUsage = 'true'"!]
[!VAR "DefensiveBehaviorEnabled" = "WdgMGeneral/WdgMDefensiveBehavior = 'true'"!]
[!VAR "CallerIdsEnabled" = "node:exists(WdgMGeneral/WdgMCallerIds)"!]
[!VAR "SleepModeEnabled" = "node:exists(WdgMConfigSet/*[1]/WdgMSleepMode)"!]
[!VAR "ServiceAPIEnabled" = "node:exists(WdgMGeneral/WdgMServiceAPI)"!]
[!VAR "SupervisorCalloutsEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts)"!]
[!VAR "GetExpectedInitStateCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetExpectedInitStateCallout)"!]
[!VAR "GetExpectedWdgMModeCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetExpectedWdgMModeCallout)"!]
[!VAR "GetTimeCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetElapsedTimeCallout)"!]
[!VAR "IsPerformResetCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMIsPerformResetCallout)"!]
[!VAR "SupervisionExpiredCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMSupervisionExpiredCallout)"!]
[!VAR "IndividualModeSwitchCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMIndividualModeSwitchCallout)"!]
[!VAR "GlobalModeSwitchCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMGlobalModeSwitchCallout)"!]
[!VAR "DetCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMDetCallout)"!]
[!VAR "WdgMPartitioningEnabled" = "WdgMGeneral/WdgMPartitioningEnabled = 'true'"!]
[!VAR "WdgMMulticoreEnabled" = "node:exists(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores) and as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores > 1"!]
[!VAR "WdgMSetModeSynchronEnabled" = "node:exists(WdgMGeneral/WdgMSetModeSynchron) and WdgMGeneral/WdgMSetModeSynchron = 'true'"!]

[!IF "$ServiceAPIEnabled"!]
[!VAR "ActivateAliveSupervisionAPIEnabled" = "node:exists(WdgMGeneral/WdgMServiceAPI/WdgMEnableASR32ActivateAliveSupervisionAPI)"!]
[!VAR "DeactivateAliveSupervisionAPIEnabled" = "node:exists(WdgMGeneral/WdgMServiceAPI/WdgMEnableASR32DeactivateAliveSupervisionAPI)"!]
[!VAR "ASR32ServiceAPIEnabled" = "WdgMGeneral/WdgMServiceAPI/WdgMEnableASR32ServiceAPI = 'true'"!]
[!VAR "ASR40ServiceAPIEnabled" = "WdgMGeneral/WdgMServiceAPI/WdgMEnableASR40ServiceAPI = 'true'"!]
[!VAR "ASR43ServiceAPIEnabled" = "WdgMGeneral/WdgMServiceAPI/WdgMEnableASR43ServiceAPI = 'true'"!]
[!VAR "DefaultServiceAPI" = "node:value(WdgMGeneral/WdgMServiceAPI/WdgMDefaultASRServiceAPI)"!]

[!ELSE!]
[!VAR "ASR32ServiceAPIEnabled" = "'false'"!]
[!VAR "ASR40ServiceAPIEnabled" = "'false'"!]
[!VAR "ASR43ServiceAPIEnabled" = "'false'"!]
[!VAR "ActivateAliveSupervisionAPIEnabled" = "'false'"!]
[!VAR "DeactivateAliveSupervisionAPIEnabled" = "'false'"!]
[!VAR "DefaultServiceAPI" = "'NONE'"!]
[!ENDIF!]

[!IF "$SupervisorCalloutsEnabled"!]
[!VAR "InitRedirectionCalloutAPIEnabled" = "not(node:empty(WdgMGeneral/WdgMSupervisorCallouts/WdgMInitRedirectionCallout))"!]
[!VAR "DeInitRedirectionCalloutAPIEnabled" = "not(node:empty(WdgMGeneral/WdgMSupervisorCallouts/WdgMDeInitRedirectionCallout))"!]
[!VAR "SetModeRedirectionCalloutAPIEnabled" = "not(node:empty(WdgMGeneral/WdgMSupervisorCallouts/WdgMSetModeRedirectionCallout))"!]
[!ELSE!]
[!VAR "InitRedirectionCalloutAPIEnabled" = "'false'"!]
[!VAR "DeInitRedirectionCalloutAPIEnabled" = "'false'"!]
[!VAR "SetModeRedirectionCalloutAPIEnabled" = "'false'"!]
[!ENDIF!]

[!VAR "NumberOfWdgModes" = "num:i(count(WdgMConfigSet/*[1]/WdgMMode/*))"!]
[!VAR "NumberOfWdgDrivers" = "num:i(count(WdgMGeneral/WdgMWatchdog/*))"!]
[!IF "$CallerIdsEnabled = 'true'"!]
[!VAR "NumberOfCallerIds" = "num:i(count(WdgMGeneral/WdgMCallerIds/WdgMCallerId/*))"!]
[!ELSE!]
[!VAR "NumberOfCallerIds" = "num:i(0)"!]
[!ENDIF!]

[!// Number of internal graphs is sum of configured internal graphs (for all WdgM modes)
[!VAR "NumberOfInternalGraphs" = "num:i(count(WdgMGeneral/WdgMSupervisedEntity/*[num:i(count(WdgMInternalTransition/*))>0]))"!]

[!// Number of external graphs is sum of configured external graphs (for all WdgM modes)
[!VAR "NumberOfExternalGraphs" = "num:i(count(WdgMConfigSet/*[1]/WdgMMode/*/WdgMExternalLogicalSupervision/*))"!]

[!// Number of DMs is sum of configured deadline supervision configs (for all WdgM modes)
[!VAR "NumberOfDMs" = "num:i(count(WdgMConfigSet/*[1]/WdgMMode/*/WdgMDeadlineSupervision/*))"!]

[!// EXPORTED VARIABLES (string parameters) --------------------------------------------------------

[!IF "$ActivateAliveSupervisionAPIEnabled"!]
[!VAR "ActivateAliveSupervisionAPIName" = "node:value(WdgMGeneral/WdgMServiceAPI/WdgMEnableASR32ActivateAliveSupervisionAPI)"!]
[!ELSE!]
[!VAR "ActivateAliveSupervisionAPIName" = "''"!]
[!ENDIF!]

[!IF "$DeactivateAliveSupervisionAPIEnabled"!]
[!VAR "DeactivateAliveSupervisionAPIName" = "node:value(WdgMGeneral/WdgMServiceAPI/WdgMEnableASR32DeactivateAliveSupervisionAPI)"!]
[!ELSE!]
[!VAR "DeactivateAliveSupervisionAPIName" = "''"!]
[!ENDIF!]

[!IF "$InitRedirectionCalloutAPIEnabled"!]
[!VAR "InitRedirectionCalloutAPIName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMInitRedirectionCallout)"!]
[!ELSE!]
[!VAR "InitRedirectionCalloutAPIName" = "''"!]
[!ENDIF!]

[!IF "$DeInitRedirectionCalloutAPIEnabled"!]
[!VAR "DeInitRedirectionCalloutAPIName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMDeInitRedirectionCallout)"!]
[!ELSE!]
[!VAR "DeInitRedirectionCalloutAPIName" = "''"!]
[!ENDIF!]

[!IF "$SetModeRedirectionCalloutAPIEnabled"!]
[!VAR "SetModeRedirectionCalloutAPIName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMSetModeRedirectionCallout)"!]
[!ELSE!]
[!VAR "SetModeRedirectionCalloutAPIName" = "''"!]
[!ENDIF!]

[!IF "$GetExpectedInitStateCalloutEnabled"!]
[!VAR "GetExpectedInitStateCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetExpectedInitStateCallout)"!]
[!ELSE!]
[!VAR "GetExpectedInitStateCalloutName" = "''"!]
[!ENDIF!]

[!IF "$GetExpectedWdgMModeCalloutEnabled"!]
[!VAR "GetExpectedWdgMModeCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetExpectedWdgMModeCallout)"!]
[!ELSE!]
[!VAR "GetExpectedWdgMModeCalloutName" = "''"!]
[!ENDIF!]

[!IF "$GetTimeCalloutEnabled"!]
[!VAR "GetTimeCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetElapsedTimeCallout)"!]
[!VAR "TimeGranularity" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMTimeGranularity)"!]
[!VAR "MainFunctionCycleTime" = "num:i(round(num:f(num:div(WdgMConfigSet/*[1]/WdgMMode/*[1]/WdgMSupervisionCycle, WdgMGeneral/WdgMSupervisorCallouts/WdgMTimeGranularity))))"!]
[!IF "$MainFunctionCycleTime = 0"!]
[!VAR "MainFunctionCycleTime" = "num:i(1)"!]
[!ENDIF!]
[!VAR "MainFunctionToleranceTime" = "num:i(round(num:f(num:div(node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMMainFunctionPeriodTolerance),node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMTimeGranularity)))))"!]
[!ELSE!]
[!VAR "GetTimeCalloutName" = "''"!]
[!VAR "MainFunctionCycleTime" = "num:i(0)"!]
[!VAR "MainFunctionToleranceTime" = "num:i(0)"!]
[!ENDIF!]

[!IF "$IsPerformResetCalloutEnabled"!]
[!VAR "IsPerformResetCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMIsPerformResetCallout)"!]
[!ELSE!]
[!VAR "IsPerformResetCalloutName" = "''"!]
[!ENDIF!]

[!IF "$SupervisionExpiredCalloutEnabled"!]
[!VAR "SupervisionExpiredCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMSupervisionExpiredCallout)"!]
[!ELSE!]
[!VAR "SupervisionExpiredCalloutName" = "''"!]
[!ENDIF!]

[!IF "$IndividualModeSwitchCalloutEnabled"!]
[!VAR "IndividualModeSwitchCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMIndividualModeSwitchCallout)"!]
[!ELSE!]
[!VAR "IndividualModeSwitchCalloutName" = "''"!]
[!ENDIF!]

[!IF "$GlobalModeSwitchCalloutEnabled"!]
[!VAR "GlobalModeSwitchCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMGlobalModeSwitchCallout)"!]
[!ELSE!]
[!VAR "GlobalModeSwitchCalloutName" = "''"!]
[!ENDIF!]

[!IF "$DetCalloutEnabled"!]
[!VAR "DetCalloutName" = "node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMDetCallout)"!]
[!ELSE!]
[!VAR "DetCalloutName" = "''"!]
[!ENDIF!]

[!IF "$WdgMMulticoreEnabled"!]
[!VAR "GetCoreIdCalloutEnabled" = "node:exists(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetCoreIdCallout)"!]
   [!IF "$GetCoreIdCalloutEnabled"!]
   [!VAR "GetCoreIdCalloutName" = "concat(node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetCoreIdCallout),'()')"!]
   [!VAR "GetCoreIdCalloutNameExtern" = "concat(node:value(WdgMGeneral/WdgMSupervisorCallouts/WdgMGetCoreIdCallout),'(void)')"!]
   [!ELSE!]
   [!VAR "GetCoreIdCalloutName" = "'((WdgM_EB_CoreIdType)GetCoreID())'"!]
   [!ENDIF!]
[!VAR "WdgMNumberOfCores" = "as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMNumberOfCores"!]
[!VAR "WdgMMasterCoreId" = "as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMMasterCoreId"!]
[!VAR "WdgMMasterWaitSlaveModeSwitch" = "node:value(as:modconf('WdgM')[1]/WdgMGeneral/WdgMGeneralMulticore/WdgMMasterWaitSlaveModeSwitch)"!]
[!ELSE!]
[!VAR "GetCoreIdCalloutEnabled" = "''"!]
[!VAR "GetCoreIdCalloutName" = "'0U'"!]
[!VAR "WdgMNumberOfCores" = "'1'"!]
[!VAR "WdgMMasterCoreId" = "'0'"!]
[!VAR "WdgMMasterWaitSlaveModeSwitch" = "'0'"!]
[!ENDIF!]

[!IF "$WdgMMulticoreEnabled"!]
  [!VAR "NumberOfSupervisedEntities" = "num:i(count(WdgMGeneral/WdgMSupervisedEntity/*))"!]
  [!VAR "NumberOfSupEntPerCore"="0"!]
  [!MACRO "GetNumSEPerCore", "Core"!][!/*
  */!][!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!/*
      */!][!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($Core)"!][!/*
          */!][!VAR "NumberOfSupEntPerCore" = "$NumberOfSupEntPerCore + 1"!][!/*
      */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
  */!][!"num:i($NumberOfSupEntPerCore)"!][!/*
  */!][!VAR "NumberOfSupEntPerCore" = "0"!][!/*
*/!][!ENDMACRO!]
[!ELSE!]
  [!VAR "NumberOfSupervisedEntities" = "num:i(count(WdgMGeneral/WdgMSupervisedEntity/*))"!]
[!ENDIF!]

[!IF "$WdgMMulticoreEnabled"!]
  [!VAR "NumberOfCheckpoints"="num:i(count(WdgMGeneral/WdgMSupervisedEntity/*/WdgMCheckpoint/*))"!]
  [!VAR "NumberOfCheckpointsPerCore" = "0"!]
  [!MACRO "GetNumOfCheckpointsPerCore", "Core"!][!/*
  */!][!LOOP "as:modconf('WdgM')[1]/WdgMGeneral/WdgMSupervisedEntity/*"!][!/*
      */!][!IF "num:i(node:value(WdgMSupervisedEntityCoreId)) = num:i($Core)"!][!/*
          */!][!LOOP "WdgMCheckpoint/*"!][!/*
              */!][!VAR "NumberOfCheckpointsPerCore" = "$NumberOfCheckpointsPerCore + 1"!][!/*
          */!][!ENDLOOP!][!/*
      */!][!ENDIF!][!/*
  */!][!ENDLOOP!][!/*
  */!][!"num:i($NumberOfCheckpointsPerCore)"!][!/*
  */!][!VAR "NumberOfCheckpointsPerCore" = "0"!][!/*
*/!][!ENDMACRO!]
[!ELSE!]
  [!/* Number of Alive Supervisions equals number of Checkpoints) */!]
  [!VAR "NumberOfCheckpoints" = "num:i(count(WdgMGeneral/WdgMSupervisedEntity/*/WdgMCheckpoint/*))"!]
[!ENDIF!]

[!IF "$WdgMMulticoreEnabled"!]
[!VAR "WdgMMasterPositionInGlobalInitStatus" = "bit:shl(1,$WdgMMasterCoreId)"!]
[!VAR "WdgMAllCoresInitialized" = "num:i(0)"!]
[!FOR "index_i" = "0" TO "num:i($WdgMNumberOfCores)-1"!]
  [!VAR "WdgMAllCoresInitialized" = "bit:or($WdgMAllCoresInitialized,bit:shl(1,$index_i))"!]
[!ENDFOR!]
[!ENDIF!]

[!/* This macro is used to generate the mapping index betwen the SEs and CPs which are part of an external graph  */!][!//

[!MACRO "GetSEIndexMapped", "SEIdCPIdModeId"!][!/*
    */!][!VAR "tmpDest"!][!ENDVAR!][!/*
    */!][!VAR "LocalVarForCheckDest"!][!ENDVAR!][!/*
    */!][!VAR "LocalVarForCheckSameSEId"!][!ENDVAR!][!/*

    */!][!/* go trough all modes */!][!/*
    */!][!LOOP "node:order(as:modconf('WdgM')[1]/WdgMConfigSet/*[1]/WdgMMode/*, 'WdgMModeId')"!][!/*
        */!][!VAR "Local_WdgMModeId" = "num:i(WdgMModeId)"!][!/*

        */!][!/* go trough all external logical supervision */!][!/*
        */!][!LOOP "WdgMExternalLogicalSupervision/*"!][!/*
            */!][!VAR "index_j" = "0"!][!/*
            */!][!VAR "tmpSameId" = "num:i(as:ref(./WdgMExternalCheckpointInitialRef/*[1])/../../WdgMSupervisedEntityId)"!][!/*

           */!][!/* get all initial checkpoint references and save the SEId, CPId, ModeId and the index (the index indicates
                at what position must be written the sequence number in WdgM_EB_ExtGraphDataSeq_GRAPHID array) */!][!/*
            */!][!LOOP "WdgMExternalCheckpointInitialRef/*"!][!/*

                */!][!VAR "FlagFindSameId" = "0"!][!/*
                */!][!/* LocalVarForCheckSameSEId variable is used to store the current SEId */!][!/*
                */!][!VAR "LocalVarForCheckSameSEId"!][!"num:i(as:ref(.)/../../WdgMSupervisedEntityId)"!][!ENDVAR!][!/*
                */!][!VAR "IndexIFtmpSameId" = "0"!][!/*

                */!][!/* go trough all tmpSameId elements and check if current SEId is already in the tmpSameId */!][!/*
                */!][!LOOP "text:split($tmpSameId,';')"!][!/*
                    */!][!IF ". = $LocalVarForCheckSameSEId"!][!/*
                        */!][!VAR "FlagFindSameId" = "1"!][!/*
                    */!][!ENDIF!][!/*
                */!][!ENDLOOP!][!/*

                */!][!/* if SEId is not found in tmpSameId, add to the tmpSameId current SEId */!][!/*
                */!][!IF "num:i($FlagFindSameId) = 0"!][!/*
                    */!][!VAR "tmpSameId"!][!"$tmpSameId"!];[!"$LocalVarForCheckSameSEId"!][!ENDVAR!][!/*

                */!][!/* if SEId is found in tmpSameId */!][!/*
                */!][!ELSE!][!/*
                    */!][!/* go trough all tmpDest elements and check if current SEId is already in the tmpDest */!][!/*
                    */!][!LOOP "text:split($tmpDest,';')"!][!/*
                        */!][!LOOP "text:split(.,'-')"!][!/*
                            */!][!IF ". = $LocalVarForCheckSameSEId"!][!/*
                                */!][!VAR "Flag" = "1"!][!/*
                            */!][!ENDIF!][!/*

                            */!][!/* if current SEId is already in tmpDest, get the index */!][!/*
                            */!][!IF "(. != num:i($LocalVarForCheckSameSEId)) and (num:i($Flag) = 1)"!][!/*
                                */!][!VAR "Flag" = "0"!][!/*
                                */!][!LOOP "text:split(.,'+')"!][!/*
                                    */!][!/* get the index */!][!/*
                                    */!][!VAR "IndexIFtmpSameId" = "."!][!/*
                                */!][!ENDLOOP!][!/*
                            */!][!ENDIF!][!/*
                        */!][!ENDLOOP!][!/*
                    */!][!ENDLOOP!][!/*
                */!][!ENDIF!][!/*

                */!][!/* if current SEId already in the list use the same index for current CP */!][!/*
                */!][!IF "num:i($FlagFindSameId) = 1"!][!/*
                    */!][!VAR "tmpDest"!][!"$tmpDest"!];[!"num:i(as:ref(.)/../../WdgMSupervisedEntityId)"!]-[!"num:i(as:ref(.)/WdgMCheckpointId)"!]:[!"num:i($Local_WdgMModeId)"!]+[!"num:i($IndexIFtmpSameId)"!][!ENDVAR!][!/*
                */!][!ELSE!][!/*
                    */!][!/* if current SEId is not in the tmpDest, increment index */!][!/*
                    */!][!VAR "index_j" = "$index_j + 1"!][!/*
                    */!][!VAR "tmpDest"!][!"$tmpDest"!];[!"num:i(as:ref(.)/../../WdgMSupervisedEntityId)"!]-[!"num:i(as:ref(.)/WdgMCheckpointId)"!]:[!"num:i($Local_WdgMModeId)"!]+[!"num:i($index_j)"!][!ENDVAR!][!/*
                */!][!ENDIF!][!/*

            */!][!ENDLOOP!][!/*

            */!][!/* go trough all external transition and save the SEId, CPId and ModeId */!][!/*
            */!][!LOOP "WdgMExternalTransition/*"!][!/*
                */!][!VAR "FlagFindDestIntmpDest" = "0"!][!/*
                */!][!VAR "FlagFindSameId" = "0"!][!/*
                */!][!VAR "Flag" = "0"!][!/*
                */!][!VAR "IndexIFtmpSameId" = "0"!][!/*

                */!][!/* LocalVarForCheckDest variable is used to store the current SEId, CPId and ModeId */!][!/*
                */!][!VAR "LocalVarForCheckDest"!][!"num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId)"!]-[!"num:i(as:ref(WdgMExternalTransitionDestRef)/WdgMCheckpointId)"!]:[!"num:i($Local_WdgMModeId)"!][!ENDVAR!][!/*
                */!][!/* LocalVarForCheckSameSEId variable is used to store the current SEId */!][!/*
                */!][!VAR "LocalVarForCheckSameSEId"!][!"num:i(as:ref(WdgMExternalTransitionDestRef)/../../WdgMSupervisedEntityId)"!][!ENDVAR!][!/*

                */!][!/* go trough all tmpDest elements and check if current SEId, CPId and ModeId are already in the tmpDest */!][!/*
                */!][!LOOP "text:split($tmpDest,';')"!][!/*
                    */!][!LOOP "text:split(.,'+')"!][!/*
                        */!][!IF ". = $LocalVarForCheckDest"!][!/*
                            */!][!VAR "FlagFindDestIntmpDest" = "1"!][!/*
                        */!][!ENDIF!][!/*
                    */!][!ENDLOOP!][!/*
                */!][!ENDLOOP!][!/*

                */!][!/* go trough all tmpSameId elements and check if current SEId is already in the tmpSameId */!][!/*
                */!][!LOOP "text:split($tmpSameId,';')"!][!/*
                    */!][!IF ". = $LocalVarForCheckSameSEId"!][!/*
                        */!][!VAR "FlagFindSameId" = "1"!][!/*
                    */!][!ENDIF!][!/*
                */!][!ENDLOOP!][!/*

                */!][!/* if SEId is not found in tmpSameId, add to the tmpSameId current SEId */!][!/*
                */!][!IF "num:i($FlagFindSameId) = 0"!][!/*
                    */!][!VAR "tmpSameId"!][!"$tmpSameId"!];[!"$LocalVarForCheckSameSEId"!][!ENDVAR!][!/*

                */!][!/* if SEId is found in tmpSameId */!][!/*
                */!][!ELSE!][!/*
                    */!][!/* go trough all tmpDest elements and check if current SEId is already in the tmpDest */!][!/*
                    */!][!LOOP "text:split($tmpDest,';')"!][!/*
                        */!][!LOOP "text:split(.,'-')"!][!/*
                            */!][!IF ". = $LocalVarForCheckSameSEId"!][!/*
                                */!][!VAR "Flag" = "1"!][!/*
                            */!][!ENDIF!][!/*

                            */!][!/* if current SEId is already in tmpDest, get the index */!][!/*
                            */!][!IF "(. != num:i($LocalVarForCheckSameSEId)) and (num:i($Flag) = 1)"!][!/*
                                */!][!VAR "Flag" = "0"!][!/*
                                */!][!LOOP "text:split(.,'+')"!][!/*
                                    */!][!/* get the index */!][!/*
                                    */!][!VAR "IndexIFtmpSameId" = "."!][!/*
                                */!][!ENDLOOP!][!/*
                            */!][!ENDIF!][!/*
                        */!][!ENDLOOP!][!/*
                    */!][!ENDLOOP!][!/*
                */!][!ENDIF!][!/*

                */!][!/* if current SEId, CPId and ModeId are not in the tmpDest  */!][!/*
                */!][!IF "num:i($FlagFindDestIntmpDest) = 0"!][!/*
                    */!][!/* if current SEId is not in the tmpDest  */!][!/*
                    */!][!IF "num:i($FlagFindSameId) = 0"!][!/*
                        */!][!/* if current SEId is not in the tmpDest, add new element with the correct index */!][!/*
                        */!][!VAR "index_j" = "$index_j + 1"!][!/*
                        */!][!VAR "tmpDest"!][!"$tmpDest"!];[!"$LocalVarForCheckDest"!]+[!"num:i($index_j)"!][!ENDVAR!][!/*
                    */!][!ELSE!][!/*
                        */!][!/* if current SEId is already in the tmpDest, add new element with the correct index  */!][!/*
                        */!][!VAR "tmpDest"!][!"$tmpDest"!];[!"$LocalVarForCheckDest"!]+[!"num:i($IndexIFtmpSameId)"!][!ENDVAR!][!/*
                    */!][!ENDIF!][!/*
                */!][!ENDIF!][!/*
             */!][!ENDLOOP!][!/*
        */!][!ENDLOOP!][!/*
    */!][!ENDLOOP!][!/*

    */!][!/* search in tmpDest variable, the matching SEId, CPId and ModeId, if found, print the index mapped to this element */!][!/*
    */!][!LOOP "text:split($tmpDest,';')"!][!/*
        */!][!LOOP "text:split(.,'+')"!][!/*
            */!][!IF ". = $SEIdCPIdModeId"!][!/*
                */!][!VAR "Flag" = "1"!][!/*
            */!][!ENDIF!][!/*
        */!][!ENDLOOP!][!/*

        */!][!IF "$Flag = 1"!][!/*
            */!][!LOOP "text:split(.,'+')"!][!/*
                */!][!IF ". != $SEIdCPIdModeId"!][!/*
                    */!]  [!"."!]U[!/*
                    */!][!VAR "Flag" = "0"!][!/*
                */!][!ENDIF!][!/*
            */!][!ENDLOOP!][!/*
        */!][!ENDIF!][!/*
    */!][!ENDLOOP!][!/*
 */!][!ENDMACRO!][!//

[!// MACROS ----------------------------------------------------------------------------------------

[!// CONSISTENCY CHECKS ----------------------------------------------------------------------------

[!ENDIF!][!// multiple inclusion protection

[!ENDNOCODE!][!//