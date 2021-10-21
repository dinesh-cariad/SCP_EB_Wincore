[!/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */!][!//
[!NOCODE!]

[!//*** multiple inclusion protection ***
[!IF "not(var:defined('STBM_TIMEBASE_M'))"!]
[!VAR "STBM_TIMEBASE_M"="'true'"!]


[!INCLUDE "StbM_ConnectedTSynBusses.m"!]


[!SELECT "as:modconf('StbM')[1]"!]

[!/* This is our "jack of all trades" data collection construct (eierlegende Wollmilchsau
     in german).
     This construct is doing the following steps:
     - Loop over all sync time bases and connected BSW modules (StbMBswModules) to do the following
       for each timebase/BSW module combination:
     - ignore all modules with missing (or invalid property file)
     - check if any <Bus>TSyn module with a valid property file supports hardware time stamping
     - check if there is any EthTSyn that supports hardware timestamp and check which autosar
       behaviour will be used(4.2.2 or 4.3)
     - get the number of slaves time domains referring a time base
     - get the master time domains referring a time base and store them in "MasterList"
     - Making error checks for invalid configuration

     The variable MasterList store all masters found for all time bases and
     has the following syntax (entries are seperated by a blank, information of a entry
     itself is seperated by ':'):

     <referenced TimeBase Id>:<TimeBase name>:<Number of Masters>:<BSW module index 0>:...:<BSW module index n>

     Attention: Only masters with supported hardware timestamping are in the list.
     All other masters are handled "passive". This means the <BusTSyn> module is the caller
     and StbM the callee.

     referenced TimeBase Id: Time Base Id
     TimeBase name: Short name of the timebase configuration container
     Number of Masters: Number of <Bus>TSyn moduled referring to the time base ID
                        (without duplicates).
     BSW module index: <Number of Masters> index identifying the BSWM module (by its
                       position in container StbMBswModules).

     The varialbe "SlaveList" stores for each synchronized time base (used as index):
     string 'true': Time base has a time domain slave referring to it.
     string 'false': Time base has no time domain slave referring to it.
*/!]
[!/* !LINKSTO StbM.dsn.AdjacentLayers,1 */!]
[!VAR "MasterList" = "''"!]
[!VAR "AllMastersList" = "''"!]
[!VAR "SlaveList" = "''"!]
[!VAR "noSetModuleInBswModulesList" = "false()"!]
[!// loop over all synchronized time bases (no offset time bases)
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[not(node:exists(./StbMOffsetTimeBase))], './StbMSynchronizedTimeBaseIdentifier')"!]

  [!VAR "TimeBaseId" = "./StbMSynchronizedTimeBaseIdentifier"!]
  [!VAR "TimeBaseName" = "node:name(.)"!]
  [!VAR "SlaveReferencesCnt" = "0"!]
  [!VAR "TBSlaveReferencesCnt" = "0"!]
  [!VAR "MasterReferencesCnt" = "0"!]
  [!VAR "TBMasterReferencesCnt" = "0"!]
  [!VAR "MasterSetterCnt" = "0"!]
  [!VAR "BswModulePos" = "num:i(0)"!]
  [!VAR "NewMasterListEntry" = "''"!]
  [!VAR "moduleName" = "''"!]
  [!VAR "NewMasterEntryFound" = "false()"!]

  [!// now do the following steps for each connected BSW module
  [!LOOP "as:modconf('StbM')[1]/StbMBswModules/*"!]
    [!// memorize name of actual BSW module
    [!VAR "BswModuleName" = "node:name(.)"!]
    [!VAR "timeStampSupportBswM" = "false()"!]
    [!VAR "SetModule" = "false()"!]
    
    [!VAR "key" = "concat($BswModuleName, '.AdjacentLayerConfig.ModuleConfigName')"!]
    [!// module must have a valid adjacent.properties file
    [!// Check this by asking for the moduleconfig name.
    [!// unknown list entries are ignored, integration of such modules underlies limitations
    [!IF "ecu:has($key)"!]
    
      [!// get a list of all 'timestamping support' parameter of time domains
      [!// referring the given time domain (might be a single (global) parameter or none)
      [!LOOP "asc:getNodesFromAbsXPathGeneric($BswModuleName, 'TimeStampSupport', $TimeBaseId)"!]
        [!// check if at least one time domain supports hardware timestamping
        [!IF "node:value(.) = 'true'"!]
          [!// The actual adjacent module supports hardware timestamping
          [!VAR "timeStampSupportBswM" = "true()"!]
        [!ENDIF!]
      [!ENDLOOP!]

      [!VAR "set" = "concat($BswModuleName, '.AdjacentLayerConfig.ApiName.V1.SetGlobalTime')"!]
      [!IF "ecu:has($set)"!]
      [!VAR "SetModule" = "true()"!]
      [!ELSE!]
      [!VAR "noSetModuleInBswModulesList" = "true()"!]
      [!ENDIF!]

      [!// get a list of all slaves for the given BSW module (TimeBaseId is unused)
      [!LOOP "asc:getNodesFromAbsXPathGeneric($BswModuleName, 'TimeBaseRefsSlave', $TimeBaseId)"!]
        [!IF "node:refvalid(.)"!]
          [!VAR "id" = "node:value(concat(node:path(as:ref(.)),'/StbMSynchronizedTimeBaseIdentifier'))"!]
          [!IF "$id = $TimeBaseId"!]
            [!// if the slave points to our actual time base increment the counter
            [!VAR "SlaveReferencesCnt" = "$SlaveReferencesCnt + 1"!]
          [!ENDIF!]
        [!ENDIF!]
      [!ENDLOOP!]
      
      [!// get a list of all masters for the given BSW module (TimeBaseId is unused)
      [!LOOP "asc:getNodesFromAbsXPathGeneric($BswModuleName, 'TimeBaseRefsMaster', $TimeBaseId)"!]
        [!IF "node:refvalid(.)"!]
        [!VAR "id" = "node:value(concat(node:path(as:ref(.)),'/StbMSynchronizedTimeBaseIdentifier'))"!]
          [!// check if the master belongs to the actual time base
          [!IF "$id = $TimeBaseId"!]
            [!VAR "MasterReferencesCnt" = "$MasterReferencesCnt + 1"!]
            [!VAR "AllMastersList" = "concat($AllMastersList, ':', $TimeBaseId)"!]
            
            [!/* Add the actual master to our "MasterList" if the following rules applies:
                 - the master supports SetGlobalTime API (else we don't call the master
                   via the SetGlobalTime API).
                 - the master isn't already in the list (avoid duplicates)
               */!]
            [!IF "($timeStampSupportBswM = 'true') and ($SetModule = 'true') and (not(contains($NewMasterListEntry, concat(':', $BswModulePos))))"!]
              [!VAR "NewMasterListEntry" = "$BswModulePos"!]
              [!VAR "NewMasterEntryFound" = "true()"!]
              [!VAR "MasterSetterCnt" = "$MasterSetterCnt + 1"!]
              [!VAR "set" = "concat($BswModuleName, '.AdjacentLayerConfig.ApiName.V1.GetCurrentTime')"!]
              [!IF "ecu:has($set)"!]
                [!VAR "moduleName" = "concat($moduleName, ':', $BswModuleName)"!]
              [!ENDIF!]
            [!ENDIF!]
            
          [!ENDIF!]
        [!ENDIF!]
      [!ENDLOOP!]
      
      [!VAR "BswModulePos" = "num:i($BswModulePos + 1)"!]
      
    [!ENDIF!]

    [!VAR "TBSlaveReferencesCnt" = "$TBSlaveReferencesCnt + $SlaveReferencesCnt"!]
    [!VAR "TBMasterReferencesCnt" = "$TBMasterReferencesCnt + $MasterReferencesCnt"!]
    [!VAR "SlaveReferencesCnt" = "0"!]
    [!VAR "MasterReferencesCnt" = "0"!]
  [!ENDLOOP!]

  [!// if we have an new entry for our master list --> concat it now
  [!IF "$NewMasterEntryFound = 'true'"!]
    [!VAR "MasterList" = "concat($MasterList, ' ', $TimeBaseId, ':', $TimeBaseName, ':', num:i($MasterSetterCnt), ':', $NewMasterListEntry, $moduleName)"!]
  [!ENDIF!]
  [!// store if the current time base has a slave connected to it (used for DET check
  [!IF "$TBSlaveReferencesCnt > 0"!]
    [!VAR "SlaveList" = "concat($SlaveList, ' ', 'true')"!]
  [!ELSE!]
    [!VAR "SlaveList" = "concat($SlaveList, ' ', 'false')"!]
  [!ENDIF!]


  [!// ===== ERROR CECHKS FOR EACH TIMEBASE LOOP ======
  [!// if a time base is without function (no connected master or slave) throw a warning
  [!/* !LINKSTO StbM.EB.Check.No_Connected_Time_Domain,1 */!]
  [!IF "($TBSlaveReferencesCnt = 0) and ($TBMasterReferencesCnt = 0)"!]
    [!IF "$TimeBaseId < 32"!]
      [!WARNING!]No time domain (neither master nor slave) refer to the time base with ID [!"$TimeBaseId"!]. This time base might be incomplete integrated to your ECU project.[!ENDWARNING!]
    [!ENDIF!]
  [!ELSE!]
    [!// a maximum of 1 slave is allowed for each time base
    [!/* !LINKSTO StbM.EB.Check.Number_Of_Slaves_Exceeded,1 */!]
    [!IF "$TBSlaveReferencesCnt > 1"!]
      [!ERROR!][!"num:i($TBSlaveReferencesCnt)"!] slave time domains reference the time base with ID [!"$TimeBaseId"!]. A maximum of one time domain configured as slave per time base is allowed.[!ENDERROR!]
    [!ENDIF!]

    [!// Time leap supervision is only done for slave time bases
    [!/* !LINKSTO StbM.EB.Check.Wrong_Usage_Of_Time_Leap_Supervision,2 */!]
    [!IF "($TBSlaveReferencesCnt = 0) and ((node:exists(StbMTimeLeapFutureThreshold)) or (node:exists(StbMTimeLeapPastThreshold)))"!]
      [!WARNING!]Ignoring time leap supervision (StbMTimeLeapFutureThreshold or StbMTimeLeapPastThreshold) of time base with ID [!"$TimeBaseId"!]. Time leap supervision is not applicable for a master time base.[!ENDWARNING!]
    [!ENDIF!]
    [!// Timeout supervision is only done for slave time bases
    [!/* !LINKSTO StbM.EB.Check.Wrong_Usage_Of_Timeout_Supervision,1 */!]
    [!IF "($TBSlaveReferencesCnt = 0) and (node:exists(StbMSyncLossTimeout))"!]
      [!WARNING!]Ignoring timeout supervision (StbMSyncLossTimeout) of time base with ID [!"$TimeBaseId"!]. Timeout supervision is not applicable for a master time base.[!ENDWARNING!]
    [!ENDIF!]
    
    [!// Slave Rate Correction and Offset Correction is only done for Sync slave time bases
    [!IF "$TimeBaseId < 16"!]
      [!// Offset Correction cannot be used without Slave Rate Correction, that's why only the check for Slave Rate Correction it's done
      [!IF "($TBSlaveReferencesCnt = 0) and (node:exists(StbMTimeCorrection))"!]
        [!// Slave Rate Correction is only done for slave time bases
        [!/* !LINKSTO StbM.EB.Check.SlaveRateCorrection_Enabled_OnMaster,1 */!]
        [!IF "(((node:exists(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration)) and (node:value(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) != 0)) or ((node:exists(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration)) and (node:value(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration) != 0)))"!]
          [!ERROR!]Ignoring slave rate correction (StbMTimeCorrection) of time base with ID [!"$TimeBaseId"!]. Slave Rate Correction is not applicable for a master time base.[!ENDERROR!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDIF!]


  [!// ===== ERROR CHECKS FOR PURE TIMEBASES ======
  [!// If Pure TimeBase is referred from TSyn modules, throw an error
  [!/* !LINKSTO StbM.EB.Check.PureTimeBase_ReferredFrom_TSyn,1 */!]
  [!IF "($TimeBaseId >= 32) and (($TBMasterReferencesCnt > 0) or ($TBSlaveReferencesCnt > 0))"!]
    [!ERROR!] The Pure Local TimeBase with ID [!"$TimeBaseId"!] cannot be referred from <bus>TSyn modules.[!ENDERROR!]
  [!ENDIF!]

  [!// ===== ERROR CHECKS FOR GPT TIME BASES ======
  [!// If StbM TimeBase uses Gpt and Gpt_GetTimeElasped API is not active in Gpt configuration, throw a warning
  [!/* !LINKSTO StbM.EB.Check.Gpt_GetTimeElasped.NotActive,1 */!]
  [!IF "(((node:value(./StbMUseOSGetTimeStamp) = 'false') and (node:exists(./StbMLocalTimeClock/StbMLocalTimeHardware))) and ((node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../GptChannelConfiguration)) and (node:value(as:modconf('Gpt')[1]/GptConfigurationOfOptApiServices/GptTimeElapsedApi) = 'false')))"!]
    [!WARNING!] GptTimeElapsedApi must be set to true if TimeBase with [!"$TimeBaseId"!] uses Gpt as time reference.[!ENDWARNING!]
  [!ENDIF!]


[!ENDLOOP!]




[!/*
****************************************************************************************************
* Check if a Pure TimeBase is referenced by an Offset TimeBase
****************************************************************************************************
*/!]
[!// loop through all Offset TimeBases
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[node:exists(./StbMOffsetTimeBase)], './StbMSynchronizedTimeBaseIdentifier')"!]
  [!VAR "PduRef" = "node:path(node:ref(./StbMOffsetTimeBase))"!]
  [!VAR "PureReferencedByOffset" = "false()"!]

  [!// loops through all pure time bases
  [!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(not(node:exists(./StbMOffsetTimeBase))) and (node:value(./StbMSynchronizedTimeBaseIdentifier) >= 32)], './StbMSynchronizedTimeBaseIdentifier')"!]
    [!VAR "PureTimeBaseName" = "node:name(.)"!]
    [!VAR "TimeBaseNameToRef" = "concat('/AUTOSAR/TOP-LEVEL-PACKAGES/StbM/ELEMENTS/StbM/StbMSynchronizedTimeBase/', $PureTimeBaseName)"!]
    [!// check is some offset from the offset list is refering a pure time base id
    [!IF "$PduRef = $TimeBaseNameToRef"!]
      [!VAR "PureReferencedByOffset" = "true()"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]

  [!// if there is at least one offset reference pointing to a pure time base
  [!/* !LINKSTO StbM.EB.Check.Offset_LinkedTo_PureTimeBase,1 */!]
  [!IF "$PureReferencedByOffset = 'true'"!]
    [!ERROR!] The Pure TimeBase '[!"$PureTimeBaseName"!]' cannot be referenced by an Offset TimeBase! [!ENDERROR!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]







[!IF "node:exists(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold)"!]

  [!VAR "gptTimerRef" = "node:path(node:ref(as:modconf('StbM')[1]/StbMGeneral/StbMGptTimerRef))"!]
  [!VAR "gptTimerRefUsedForOtherPurposes" = "false()"!]
  
  [!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]

    [!IF "node:exists(./StbMLocalTimeClock)"!]
      [!IF "node:value(./StbMLocalTimeClock/StbMLocalTimeHardware) != '' "!]

        [!VAR "timeHardwareRef" = "node:path(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware))"!]

        [!IF "node:path($gptTimerRef) = node:path($timeHardwareRef)"!]
          [!VAR "gptTimerRefUsedForOtherPurposes" = "true()"!]
          [!BREAK!]
        [!ENDIF!]

      [!ENDIF!]
    [!ENDIF!]

  [!ENDLOOP!]
  
  
  [!// ===== CHECKING IF THE REFERENCE TO WHICH StbMGptTimerRef POINTS, IS NOT USED FOR OTHER PURPOSES ======
  [!// If StbMGptTimerRef points to a GptChannel which was already used as a StbMLocalTimeHardware, throw an error
  [!/* !LINKSTO StbM.EB.Check.StbMGptTimerRef.StbMLocalTimeHardware,1 */!]
  [!IF "$gptTimerRefUsedForOtherPurposes = 'true'"!]
    [!ERROR!] The GptChannelConfiguration which is referenced by StbMGptTimerRef, shall not be used as a StbMLocalTimeHardware reference or for other purposes![!ENDERROR!]
  [!ENDIF!]
  
[!ENDIF!]




[!// ====================== ERROR CHECKS FOR StbMAllowSystemWideGlobalTimeMaster AND StbMIsSystemWideGlobalTimeMaster PARAMETERS ======================
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[not(node:value(./StbMSynchronizedTimeBaseIdentifier) > 31)], './StbMSynchronizedTimeBaseIdentifier')"!]

    [!VAR "TimeBaseId" = "node:value(./StbMSynchronizedTimeBaseIdentifier)"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    
	[!/* !LINKSTO StbM.EB.Check.StbMAllowSystemWideGlobalTimeMaster_Enabled_OnSyncSlave,1 */!]
	[!/* !LINKSTO StbM.EB.Check.StbMAllowSystemWideGlobalTimeMaster_Enabled_OnOffsetSlave,1 */!]
    [!// a slave time base shall not have StbMAllowSystemWideGlobalTimeMaster activated
    [!IF "($isSlaveConnected = 'isSlave')"!]
      [!IF "(node:exists(./StbMAllowSystemWideGlobalTimeMaster)) and (node:value(./StbMAllowSystemWideGlobalTimeMaster) = 'true')"!]
        [!ERROR!] SLAVE timeBase with ID [!"$TimeBaseId"!] cannot have StbMAllowSystemWideGlobalTimeMaster set to TRUE![!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]
	
	[!/* !LINKSTO StbM.EB.Check.StbMIsSystemWideGlobalTimeMaster_Enabled_OnGatewaySync,1 */!]
	[!/* !LINKSTO StbM.EB.Check.StbMIsSystemWideGlobalTimeMaster_Enabled_OnGatewayOffset,1 */!]
	[!// a slave time base shall not have StbMAllowSystemWideGlobalTimeMaster activated
    [!IF "($isMasterSlaveConnected = 'true')"!]
      [!IF "(((node:exists(./StbMAllowSystemWideGlobalTimeMaster)) and (node:value(./StbMAllowSystemWideGlobalTimeMaster) = 'true')) and (node:value(./StbMIsSystemWideGlobalTimeMaster) = 'true'))"!]
        [!ERROR!] GATEWAY timeBase with ID [!"$TimeBaseId"!] cannot have StbMIsSystemWideGlobalTimeMaster set to TRUE![!ENDERROR!]
      [!ENDIF!]
    [!ENDIF!]

[!ENDLOOP!]



[!// ====================== ERROR CHECKS FOR StbMAllowMasterRateCorrection PARAMETER ======================
[!// loop through all time bases
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]

    [!VAR "TimeBaseId" = "node:value(./StbMSynchronizedTimeBaseIdentifier)"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    
	[!/* !LINKSTO StbM.EB.Check.StbMAllowMasterRateCorrection_Activated_OnSyncSlave,1 */!]
	[!/* !LINKSTO StbM.EB.Check.StbMAllowMasterRateCorrection_Activated_OnOffsetSlave,1 */!]
    [!// a slave time base shall not have StbMAllowMasterRateCorrection activated
    [!IF "(($isSlaveConnected = 'isSlave') and ($isAllowMasterRateCorrectionActivatedForThisTimeBase = 'true'))"!]
      [!ERROR!] SLAVE timeBase with ID [!"$TimeBaseId"!] cannot have StbMAllowMasterRateCorrection set to TRUE![!ENDERROR!]
    [!ENDIF!]
	
	[!/* !LINKSTO StbM.EB.Check.StbMAllowMasterRateCorrection_Activated_OnGatewaySync,1 */!]
	[!/* !LINKSTO StbM.EB.Check.StbMAllowMasterRateCorrection_Activated_OnGatewayOffset,1 */!]
	[!// a gateway time base shall not have StbMAllowSystemWideGlobalTimeMaster activated
    [!IF "(($isMasterSlaveConnected = 'true') and ($isAllowMasterRateCorrectionActivatedForThisTimeBase = 'true'))"!]
      [!ERROR!] GATEWAY timeBase with ID [!"$TimeBaseId"!] cannot have StbMAllowMasterRateCorrection set to TRUE![!ENDERROR!]
    [!ENDIF!]

[!ENDLOOP!]




[!// =================================================== ERROR CHECKS FOR StbMTimeValidation ==========================================================
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[not(node:value(./StbMSynchronizedTimeBaseIdentifier) >= 16)], './StbMSynchronizedTimeBaseIdentifier')"!]
    [!VAR "TimeBaseId" = "node:value(./StbMSynchronizedTimeBaseIdentifier)"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    
	[!/* !LINKSTO StbM.EB.Check.AtLeast1TimeBase.StbMTimeValidation.Enabled.StbMRteUsage.Disabled,1 */!]
    [!// Rte shall be enabled, if TimeValidation is intended to be used
    [!IF "(($isTimeValidationEnabledForThisTimeBase = 'true') and (node:value(as:modconf('StbM')[1]/StbMGeneral/StbMRteUsage) = 'false'))"!]
      [!ERROR!] If StbMTimeValidation is intended to be used for timebase with ID [!"$TimeBaseId"!], StbMRteUsage shall be enabled![!ENDERROR!]
    [!ENDIF!]
    
    [!/* !LINKSTO StbM.EB.Check.SYNCTimeBase.StbMTimeValidation.Enabled.NoTSynTimeDomain.RefersIt,1 */!]
    [!// If StbMTimeValidation is enalbed on a SYNC time base, it shall be reffered by a TSyn time domain
    [!IF "(($isTimeValidationEnabledForThisTimeBase = 'true') and (($isMasterConnected = 'noMaster') and ($isSlaveConnected = 'noSlave') and ($isMasterSlaveConnected = 'noGateway')))"!]
      [!ERROR!] If StbMTimeValidation is intended to be used for timebase with ID [!"$TimeBaseId"!], a TSyn time domain shall refer it![!ENDERROR!]
    [!ENDIF!]
[!ENDLOOP!]

[!// =================================================== ERROR CHECKS FOR StbMTimeRecording ==========================================================
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[not(node:value(./StbMSynchronizedTimeBaseIdentifier) >= 32)], './StbMSynchronizedTimeBaseIdentifier')"!]
    [!VAR "TimeBaseId" = "node:value(./StbMSynchronizedTimeBaseIdentifier)"!]
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    
    [!// TimeRecording shall not be activated on SYNC MASTER or OFFSET MASTER time bases
    [!IF "(($isMasterConnected = 'isMaster') and ($isTimeRecordingEnabled = 'true'))"!]
        [!IF "($TimeBaseId < 16)"!]
          [!/* !LINKSTO StbM.EB.Check.StbMTimeRecording.Activated.On.SyncMASTER,1 */!]
          [!ERROR!] StbMTimeRecording shall not be activated on SYNC MASTER time base id [!"$TimeBaseId"!]![!ENDERROR!]
        [!ELSE!]
          [!/* !LINKSTO StbM.EB.Check.StbMTimeRecording.Activated.On.OfsMASTER,1 */!]
          [!ERROR!] StbMTimeRecording shall not be activated on OFFSET MASTER time base id [!"$TimeBaseId"!]![!ENDERROR!]
        [!ENDIF!]
    [!ENDIF!]
[!ENDLOOP!]



[!// - EOF ----------------------
[!ENDSELECT!]
[!ENDIF!]
[!ENDNOCODE!]
