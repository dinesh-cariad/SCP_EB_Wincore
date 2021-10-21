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
[!IF "not(var:defined('STBM_TIMEBASEHLP_M'))"!]
[!VAR "STBM_TIMEBASEHLP_M"="'true'"!]


[!// at least one SYNC TimeBase has StbMTimeValidation enabled --> enable Time Validation feature
[!VAR "StbMTimeValidationUsed" = "false()"!]
[!// loop through all SYNC StbM time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(node:value(./StbMSynchronizedTimeBaseIdentifier) < 16)]"!]
  [!IF "(node:exists(./StbMTimeValidation))"!]
    [!VAR "StbMTimeValidationUsed" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!VAR "StbMTimeRecordingSupport" = "node:value(as:modconf('StbM')[1]/StbMGeneral/StbMTimeRecordingSupport)"!][!//
[!VAR "StbMMainFunctionPeriod" = "node:value(as:modconf('StbM')[1]/StbMGeneral/StbMMainFunctionPeriod)"!][!//

[!/* This list contains the following information, taken from EthTSyn Global Time Domains:

      column 1: Referenced Syncronized Time Base name
      column 2: role of the time domain: True for MASTER, False for Slave

     and it looks like this:
             1                         2
     referencedSyncTimeBase     :     role
e.g.  StbMSynchronizedTimeBase_0      true
     (An EthTSyn Time Domain with role Master is referencing a SYNC time base with name: StbMSynchronizedTimeBase_0)

*/!]
[!VAR "ListOfEthTSynSyncTimeDomains" = "''"!]

[!/* This list contains the following information, taken from EthTSyn Global Time Domains:

      column 1: Referenced Synchronized Time Base name, referenced by the Offset Time Base
      column 2: Referenced Offset Time Base if

     and it looks like this:
             1                            2                                   3                                                    4                                                5
     referencedSyncTimeBase     :     offsetTimeBaseId    allowSystemWideGlobalMasterForMasterEnabledOffset  allowSystemWideGlobalMasterForMasterUsedOffset   isSystemWideGlobalMasterForMasterUsedOffset
e.g.  StbMSynchronizedTimeBase_0          16                                  true                                              true                                               true
     (An EthTSyn Time Domain with role Master is referencing a SYNC time base with name: StbMSynchronizedTimeBase_0 + all SystemWide params are enabled)

*/!]

[!VAR "ListOfEthTSynOffsetTimeDomains" = "''"!]


[!/* This macro is used to generate the above two lists */!]
[!MACRO "CreateEthTSynTimeDomainsList"!][!NOCODE!]
[!LOOP "as:modconf('EthTSyn')[1]/EthTSynGlobalTimeDomain/*"!]

  [!VAR "allowSystemWideGlobalMasterForMasterEnabledOffset" = "false()"!]
  [!VAR "allowSystemWideGlobalMasterForMasterUsedOffset" = "false()"!]
  [!VAR "isSystemWideGlobalMasterForMasterUsedOffset" = "false()"!]

  [!VAR "timeDomainId" = "./EthTSynGlobalTimeDomainId "!]

  [!IF "$timeDomainId < 16"!]
    [!IF "node:value(./EthTSynSynchronizedTimeBaseRef) != ''"!]
      [!IF "node:refvalid(./EthTSynSynchronizedTimeBaseRef)"!]
        [!VAR "referencedSyncTimeBase" = "node:name(as:ref(./EthTSynSynchronizedTimeBaseRef))"!]
        [!IF "node:exists(./EthTSynPortRole)"!]
          [!VAR "role" = "node:containsValue(./EthTSynPortRole, 'EthTSynGlobalTimeMaster')"!]
          [!VAR "ListOfEthTSynSyncTimeDomains" = "concat($ListOfEthTSynSyncTimeDomains, ' ', $referencedSyncTimeBase, ':', $role, ':', ';')"!]
        [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]
  [!ELSE!]
    [!IF "node:value(./EthTSynSynchronizedTimeBaseRef) != ''"!]
      [!IF "node:refvalid(./EthTSynSynchronizedTimeBaseRef)"!]
        [!IF "node:exists(node:value(as:ref(./EthTSynSynchronizedTimeBaseRef)/StbMOffsetTimeBase))"!]

          [!IF "node:exists(as:ref(./EthTSynSynchronizedTimeBaseRef)/StbMAllowSystemWideGlobalTimeMaster)"!]
            [!VAR "allowSystemWideGlobalMasterForMasterEnabledOffset" = "true()"!]
            [!IF "node:value(as:ref(./EthTSynSynchronizedTimeBaseRef)/StbMAllowSystemWideGlobalTimeMaster) = 'true'"!]
             [!VAR "allowSystemWideGlobalMasterForMasterUsedOffset" = "true()"!]
              [!IF "node:value(as:ref(./EthTSynSynchronizedTimeBaseRef)/StbMIsSystemWideGlobalTimeMaster) = 'true'"!]
                [!VAR "isSystemWideGlobalMasterForMasterUsedOffset" = "true()"!]
              [!ENDIF!]
            [!ENDIF!]
          [!ENDIF!]

          [!VAR "referencedSyncTimeBase" = "node:name(node:value(as:ref(./EthTSynSynchronizedTimeBaseRef)/StbMOffsetTimeBase))"!]
          [!VAR "offsetTimeBaseId" = "node:value(as:ref(./EthTSynSynchronizedTimeBaseRef)/StbMSynchronizedTimeBaseIdentifier)"!]
          [!VAR "ListOfEthTSynOffsetTimeDomains" = "concat($ListOfEthTSynOffsetTimeDomains, ' ', $referencedSyncTimeBase, ':', $offsetTimeBaseId, ':',  $allowSystemWideGlobalMasterForMasterEnabledOffset, ':',  $allowSystemWideGlobalMasterForMasterUsedOffset, ':',  $isSystemWideGlobalMasterForMasterUsedOffset, ':',';')"!]
      [!ENDIF!]
      [!ENDIF!]
    [!ENDIF!]
  [!ENDIF!]

[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]



[!/* This list contains the following information, based on EthTSyn Global Time Domains referencing Offset Time Bases:

      column 1: Referenced Synchronized Time Base name, referenced by the Offset Time Base
      column 2: Referenced Offset Time Base if

     and it looks like this:
                1                      2                                 3                                                    4                                                5
     offsetTimeBaseId      :     roleOfSyncDomain    allowSystemWideGlobalMasterForMasterEnabledOffset  allowSystemWideGlobalMasterForMasterUsedOffset   isSystemWideGlobalMasterForMasterUsedOffset
e.g.          16                     true                                true                                              true                                               true
     (The EthTSyn Time Domain referencing the Offset Time Base with id 16, has the role Master)

*/!]
[!VAR "ListOfEthTSynOffsetTimeBases" = "''"!]

[!/* This macro is used to generate the above list */!]
[!/* It only works if th CreateEthTSynTimeDomainsList macro is called before */!]
[!MACRO "CreateEthTSynOffsetTimeBasesList"!][!NOCODE!]
[!LOOP "text:split($ListOfEthTSynOffsetTimeDomains, ';')"!]

[!VAR "referencedSyncTimeBaseOffsetDomain" = "text:split( ., ':' )[1]"!]
[!VAR "offsetTimeBaseId" = "text:split( ., ':' )[2]"!]
[!VAR "allowSystemWideGlobalMasterForMasterEnabledOnOffset" = "text:split( ., ':' )[3]"!]
[!VAR "allowSystemWideGlobalMasterForMasterUsedOnOffset" = "text:split( ., ':' )[4]"!]
[!VAR "isSystemWideGlobalMasterForMasterUsedOnOffset" = "text:split( ., ':' )[5]"!]

  [!LOOP "text:split($ListOfEthTSynSyncTimeDomains, ';')"!]

    [!VAR "referencedSyncTimeBaseSyncDomain" = "text:split( ., ':' )[1]"!]
    [!VAR "roleOfSyncDomain" = "text:split( ., ':' )[2]"!]

    [!IF "$referencedSyncTimeBaseOffsetDomain = $referencedSyncTimeBaseSyncDomain"!]
      [!VAR "ListOfEthTSynOffsetTimeBases" = "concat($ListOfEthTSynOffsetTimeBases, ' ', $offsetTimeBaseId, ':', $roleOfSyncDomain, ':', $allowSystemWideGlobalMasterForMasterEnabledOnOffset, ':', $allowSystemWideGlobalMasterForMasterUsedOnOffset, ':', $isSystemWideGlobalMasterForMasterUsedOnOffset, ':',';')"!]
    [!ENDIF!]

  [!ENDLOOP!]
[!ENDLOOP!]
[!ENDNOCODE!][!ENDMACRO!]


[!VAR "isEthTSynModulePresent" = "false()"!]
[!LOOP "as:modconf('StbM')[1]/StbMBswModules/*"!]
  [!IF "node:name(.) = 'EthTSyn'"!]
    [!VAR "isEthTSynModulePresent" = "true()"!]
  [!ENDIF!]
[!ENDLOOP!]


[!// check if EthTSyn is present in the configuration
[!IF "$isEthTSynModulePresent = 'true'"!]
  [!// prepare Offset Time Bases List
  [!// since Offset Time Domains in EthTSyn do not have a EthTSynPortRole we need to obtain it from the Sync Time Base linked to the Offset Time Domain
  [!// (through the Offset Time Base reference)
  [!CALL "CreateEthTSynTimeDomainsList"!]
  [!CALL "CreateEthTSynOffsetTimeBasesList"!]

[!ENDIF!]


[!VAR "isStatusNotificationUsed" = "false()"!]
[!// loop over all time bases
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
  [!IF "((node:value(./StbMNotificationInterface) = 'SR_INTERFACE') or (node:value(./StbMNotificationInterface) = 'CALLBACK_AND_SR_INTERFACE'))"!]
    [!VAR "isStatusNotificationUsed" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]


[!SELECT "as:modconf('StbM')[1]"!]

[!// Number of sync time bases in StbM configuration
[!VAR "StbMSyncTimebaseNumber" = "num:i(count(StbMSynchronizedTimeBase/*[node:value(StbMSynchronizedTimeBaseIdentifier) < 16]))"!]

[!// Number of offset time bases in StbM configuration
[!VAR "StbMOffsetTimebaseNumber" = "num:i(count(StbMSynchronizedTimeBase/*[node:exists(StbMOffsetTimeBase)]))"!]

[!// Number of offset time bases in StbM configuration
[!VAR "StbMExtendedTimeStampUsed" = "node:value(StbMGeneral/StbMGetCurrentTimeExtendedAvailable) = 'true'"!]


[!/* This list contains the following information:
      column 1: TimeBaseId
      column 2: is there a MASTER TimeDomain connected
      column 3: is there a SLAVE TimeDomain connected
      column 4: is there a MASTER and SLAVE connected
      column 5: is there a CAN Master connected
      column 6: is there a FR Master connected
      column 7: is there an ETH Master connected
      column 8: is there an ETH PDelay Responder connected
      column 9: is there a CAN Slave connected
      column 10: is there a FR Slave connected
      column 11: is there an ETH Slave connected
      column 12: is there an ETH PDelay Initiator connected
      column 13: time base name
      column 14: is Time Validation enabled on time base
      column 15: is Time Recording enabled on time base
	  column 16: is allowSystemWide used on this time base
	  column 17: is isSystemWide used on time base
	  column 18: is TimeRecording used on this time base  
      column 19: is allowSystemWide enabled on this time base
      column 20: is statusNotification used on this time base
      column 21: is allowMasterRateCorrection activated on this time base
     and it looks like this:
      1          2                    3               4               5                      6                    7                               8                             9                    10                   11                           12                           13                             14                             15                                  16                                      17                                       18                19                               20                                        21
     ID : isMasterConnected : isSlaveConnected : isGateway : isMasterCANConnected : isMasterFRConnected : isMasterETHConnected : isMasterETHPDelayResponderConnected : isSlaveCANConnected : isSlaveFRConnected : isSlaveETHConnected : isSlaveETHPDelayInitiatorConnected : stbMTimeBaseName :            isTimeValidationEnabled  :    isTimeRecordingEnabled     :    allowSystemWideGlobalMasterForMaster  :   isSystemWideGlobalMasterForMaster  :  isTimeRecordingUsed  :    isAllowSystemWideUsed  :   isStatusNotificationUsedOnTimeBase :  isAllowMasterRateCorrectionActivatedOnTimeBase
e.g. 14       noMaster               noSlave        true         masterCanTSyn         noFrTSynMaster        noEthTSynMaster          noEthTSynPdelayResponder             noCanTSynSlave          slaveFrTSyn        noEthTSynSlave          noEthTSynPdelayInitiator       StbM_SynchronizedTimeBase_4              true                          true                           false                                     false                                  false                false                         false                                         false
     (An StbM TimeBase with ID = 4 has a Master CAN connected and also a Slave FR connected, that means that our TimeBase is a Gateway)

*/!]
[!VAR "ListOfBusConnectionsToTimeBase" = "''"!]

[!// loop over all time bases
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*, './StbMSynchronizedTimeBaseIdentifier')"!]
  [!VAR "TimeBaseId" = "./StbMSynchronizedTimeBaseIdentifier"!]
  
  [!VAR "isTimeRecordingEnabledOnTimeBase" = "false()"!]
  [!VAR "isTimeValidationEnabled" = "false()"!]
  [!VAR "isTimeRecordingUsedOnTimeBase" = "false()"!]
  [!VAR "isStatusNotificationUsedOnTimeBase" = "false()"!]
  
  [!VAR "isSlaveConnected" = "'noSlave'"!]
  [!VAR "isMasterConnected" = "'noMaster'"!]
  [!VAR "isGateway" = "'noGateway'"!]

  [!VAR "foundInMasterCanTSyn" = "'noCanTSynMaster'"!]
  [!VAR "foundInMasterFrTSyn" = "'noFrTSynMaster'"!]
  [!VAR "foundInMasterEthTSyn" = "'noEthTSynMaster'"!]
  [!VAR "foundInMasterEthTSynPDelayResp" = "'noEthTSynMasterPdelayResponder'"!]
  [!VAR "foundInSlaveCanTSyn" = "'noCanTSynSlave'"!]
  [!VAR "foundInSlaveFrTSyn" = "'noFrTSynSlave'"!]
  [!VAR "foundInSlaveEthTSyn" = "'noEthTSynSlave'"!]
  [!VAR "foundInSlaveEthTSynPDelayInit" = "'noEthTSynSlavePdelayInitiator'"!]

  [!VAR "allowSystemWideGlobalMasterForMasterEnabled" = "false()"!]
  [!VAR "allowSystemWideGlobalMasterForMasterUsed" = "false()"!]
  [!VAR "isSystemWideGlobalMasterForMasterUsed" = "false()"!]
  
  [!VAR "isAllowMasterRateCorrectionActivatedOnTimeBase" = "false()"!]
  
  [!IF "((node:exists(./StbMTimeCorrection)) and (node:exists(./StbMTimeCorrection/StbMAllowMasterRateCorrection)))"!]
  [!VAR "isAllowMasterRateCorrectionActivatedOnTimeBase" = "node:value(./StbMTimeCorrection/StbMAllowMasterRateCorrection)"!]
  [!ENDIF!]

  [!VAR "isTimeValidationEnabled" = "(node:exists(./StbMTimeValidation))"!]

  [!IF "node:exists(./StbMTimeRecording)"!]
    [!VAR "isTimeRecordingEnabledOnTimeBase" = "true()"!]
    [!IF "$TimeBaseId < 16"!]
	  [!VAR "isTimeRecordingUsedOnTimeBase" = "(node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) > 0)"!]
    [!ELSE!]
      [!VAR "isTimeRecordingUsedOnTimeBase" = "(node:value(./StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount) > 0)"!]
    [!ENDIF!]
  [!ENDIF!]
  
  [!IF "((node:value(./StbMNotificationInterface) = 'SR_INTERFACE') or (node:value(./StbMNotificationInterface) = 'CALLBACK_AND_SR_INTERFACE'))"!]
    [!VAR "isStatusNotificationUsedOnTimeBase" = "true()"!]
  [!ENDIF!]

  [!// check if Time Base is a MASTER, SLAVE or GATEWAY only for SYNC and OFFSET time bases, because PURE time bases are always considered MASTERs
  [!IF "$TimeBaseId <= 31"!]
  
  [!// now do the following steps for each connected BSW module
  [!LOOP "as:modconf('StbM')[1]/StbMBswModules/*"!]
    [!VAR "BswModuleName" = "node:name(.)"!]
    [!VAR "key" = "concat($BswModuleName, '.AdjacentLayerConfig.ModuleConfigName')"!]
    [!// module must have a valid adjacent.properties file
    [!// Check this by asking for the moduleconfig name.
    [!// unknown list entries are ignored, integration of such modules underlies limitations
    [!IF "ecu:has($key)"!]

      [!// EthTSyn Offset Time Bases information can not be obtained from the adjacent.properties file
      [!// So it needs to be obtained from the above created lists
      [!IF "$TimeBaseId > 15"!]
        [!IF "$BswModuleName = 'EthTSyn'"!]
          [!LOOP "text:split($ListOfEthTSynOffsetTimeBases, ';')"!]
            [!VAR "offsetId" = "num:i(text:split( ., ':' )[1])"!]
            [!VAR "offsetRole" = "text:split( ., ':' )[2]"!]

              [!// check if our OffsetId is part of the list
              [!IF "$TimeBaseId = $offsetId"!]
                [!// check if it is a Master or not
                [!IF "$offsetRole ='true'"!]
                  [!VAR "isMasterConnected" = "'isMaster'"!]
                  [!VAR "foundInMasterEthTSyn" = "'masterEthTSyn'"!]

                    [!VAR "allowSystemWideGlobalMasterForMasterEnabled" = "text:split( ., ':' )[3]"!]
                    [!VAR "allowSystemWideGlobalMasterForMasterUsed" = "text:split( ., ':' )[4]"!]
                    [!VAR "isSystemWideGlobalMasterForMasterUsed" = "text:split( ., ':' )[5]"!]
                [!ELSE!]
                  [!VAR "isSlaveConnected" = "'isSlave'"!]
                  [!VAR "foundInSlaveEthTSyn" = "'slaveEthTSyn'"!]
                [!ENDIF!]
              [!ENDIF!]
          [!ENDLOOP!]
        [!ENDIF!]
      [!ENDIF!]

      [!// get a list of all slaves for the given BSW module (TimeBaseId is unused)
      [!LOOP "asc:getNodesFromAbsXPathGeneric($BswModuleName, 'TimeBaseRefsSlave', $TimeBaseId)"!]
        [!IF "node:refvalid(.)"!]
          [!VAR "id" = "node:value(concat(node:path(as:ref(.)),'/StbMSynchronizedTimeBaseIdentifier'))"!]
          [!IF "$id = $TimeBaseId"!]

            [!// the slave points to our actual time base
            [!VAR "isSlaveConnected" = "'isSlave'"!]

            [!// check which Slave BUS refferes our TimeBase
            [!IF "$BswModuleName = 'CanTSyn'"!]
              [!VAR "foundInSlaveCanTSyn" = "'slaveCanTSyn'"!]
            [!ENDIF!]

            [!IF "$BswModuleName = 'FrTSyn'"!]
              [!VAR "foundInSlaveFrTSyn" = "'slaveFrTSyn'"!]
            [!ENDIF!]

            [!IF "$BswModuleName = 'EthTSyn'"!]
              [!VAR "foundInSlaveEthTSyn" = "'slaveEthTSyn'"!]
              
              [!IF "((node:exists(./../EthTSynPortConfig/*[1]/EthTSynPdelayConfig/EthTSynGlobalTimeTxPdelayReqPeriod)) and (node:value(./../EthTSynPortConfig/*[1]/EthTSynPdelayConfig/EthTSynGlobalTimeTxPdelayReqPeriod) != 0))"!]
                [!VAR "foundInSlaveEthTSynPDelayInit" = "'slaveEthTSynPdelayInitiator'"!]
              [!ENDIF!]
              
            [!ENDIF!]


          [!ENDIF!]
        [!ENDIF!]
      [!ENDLOOP!]

      [!// get a list of all masters for the given BSW module (TimeBaseId is unused)
      [!LOOP "asc:getNodesFromAbsXPathGeneric($BswModuleName, 'TimeBaseRefsMaster', $TimeBaseId)"!]
        [!IF "node:refvalid(.)"!]
          [!VAR "id" = "node:value(concat(node:path(as:ref(.)),'/StbMSynchronizedTimeBaseIdentifier'))"!]
          [!// check if the master belongs to the actual time base

          [!IF "$id = $TimeBaseId"!]

            [!// the master points to our actual time base
            [!VAR "isMasterConnected" = "'isMaster'"!]
			
			[!IF "(node:exists(concat(node:path(as:ref(.)),'/StbMAllowSystemWideGlobalTimeMaster')) = 'true')"!]
              [!VAR "allowSystemWideGlobalMasterForMasterEnabled" = "true()"!]
              [!IF "(node:value(concat(node:path(as:ref(.)),'/StbMAllowSystemWideGlobalTimeMaster')) = 'true')"!]
			    [!VAR "allowSystemWideGlobalMasterForMasterUsed" = "true()"!]
                [!IF "node:value(concat(node:path(as:ref(.)),'/StbMIsSystemWideGlobalTimeMaster')) = 'true'"!]
                  [!VAR "isSystemWideGlobalMasterForMasterUsed" = "true()"!]
                [!ENDIF!]
              [!ENDIF!]
			[!ENDIF!]

            [!// check which Master BUS refferes our TimeBase
            [!IF "$BswModuleName = 'CanTSyn'"!]
              [!VAR "foundInMasterCanTSyn" = "'masterCanTSyn'"!]
            [!ENDIF!]

            [!IF "$BswModuleName = 'FrTSyn'"!]
              [!VAR "foundInMasterFrTSyn" = "'masterFrTSyn'"!]
            [!ENDIF!]

            [!IF "$BswModuleName = 'EthTSyn'"!]
              [!VAR "foundInMasterEthTSyn" = "'masterEthTSyn'"!]
            
              [!IF "((node:exists(./../EthTSynPortConfig/*[1]/EthTSynPdelayConfig/EthTSynGlobalTimePdelayRespEnable)) and (node:value(./../EthTSynPortConfig/*[1]/EthTSynPdelayConfig/EthTSynGlobalTimePdelayRespEnable) = 'true'))"!]
                [!VAR "foundInMasterEthTSynPDelayResp" = "'masterEthTSynPdelayResponder'"!]
              [!ENDIF!]
              
            [!ENDIF!]

          [!ENDIF!]

        [!ENDIF!]
      [!ENDLOOP!]

    [!ENDIF!]
  [!ENDLOOP!]
  
  [!IF "($isMasterConnected = 'isMaster') and ($isSlaveConnected = 'isSlave')"!]
    [!VAR "isMasterConnected" = "'noMaster'"!]
	[!VAR "isSlaveConnected" = "'noSlave'"!]
    [!VAR "isGateway" = "true()"!]
  [!ENDIF!]

  [!ELSE!]
    [!// PURE time bases are always considered master time bases
    [!VAR "isMasterConnected" = "'isMaster'"!]
    
    [!VAR "allowSystemWideGlobalMasterForMasterEnabled" = "true()"!]
    [!VAR "allowSystemWideGlobalMasterForMasterUsed" = "true()"!]
    [!VAR "isSystemWideGlobalMasterForMasterUsed" = "true()"!]
	
  [!ENDIF!]
  
  [!VAR "ListOfBusConnectionsToTimeBase" = "concat($ListOfBusConnectionsToTimeBase, ' ', $TimeBaseId, ':', $isMasterConnected, ':', $isSlaveConnected, ':', $isGateway, ':', $foundInMasterCanTSyn, ':', $foundInMasterFrTSyn, ':', $foundInMasterEthTSyn, ':', $foundInMasterEthTSynPDelayResp, ':', $foundInSlaveCanTSyn, ':', $foundInSlaveFrTSyn, ':', $foundInSlaveEthTSyn, ':', $foundInSlaveEthTSynPDelayInit, ':', node:name(.), ':', $isTimeValidationEnabled, ':', $isTimeRecordingEnabledOnTimeBase, ':', $allowSystemWideGlobalMasterForMasterUsed, ':', $isSystemWideGlobalMasterForMasterUsed, ':', $isTimeRecordingUsedOnTimeBase, ':', $allowSystemWideGlobalMasterForMasterEnabled, ':', $isStatusNotificationUsedOnTimeBase, ':', $isAllowMasterRateCorrectionActivatedOnTimeBase, ';')"!]

[!ENDLOOP!]





[!/* This list contains the following information:
     EcuCPartitionName
     and it looks like this:
       
     first EcuCPartitionName ; second EcuCPartitionName ; third EcuCPartitionName ; n'th EcuCPartitionName ;
     
e.g. StbM has 4 references to EcuCPartitions in StbMEcucPartitionRefList list.
     The ListOfEcucPartitionNames will look like this:
       _EcuCPartition_0; _EcuCPartition_1; _EcuCPartition_2; _EcuCPartition_3;

*/!]

[!VAR "ListOfEcucPartitionNames" = "''"!]

[!VAR "StbMMulticoreUsed" = "node:value(StbMGeneral/StbMMulticoreSupport) = 'true'"!]
[!IF "$StbMMulticoreUsed = 'true'"!]

[!// loop over all time bases
[!LOOP "as:modconf('StbM')[1]/StbMGeneral/StbMEcucPartitionRefList/*"!]
  [!// take the EcucPartition name
  [!VAR "EcuCPartitionName" = "concat('_', node:name(node:path(node:ref(./StbMEcucPartitionRef))))"!]
  [!// add it to the ListOfEcucPartitionNames list
  [!VAR "ListOfEcucPartitionNames" = "concat($ListOfEcucPartitionNames, $EcuCPartitionName, ';')"!]
[!ENDLOOP!]

[!ELSE!]

  [!// take the EcucPartition name
  [!VAR "EcuCPartitionName" = "'_NoPartition'"!]
  [!// add it to the ListOfEcucPartitionNames list
  [!VAR "ListOfEcucPartitionNames" = "concat($ListOfEcucPartitionNames, $EcuCPartitionName, ';')"!]

[!ENDIF!]




[!VAR "StbMNotificationCustomerSupport" = "node:exists(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold)"!][!//
[!IF "$StbMNotificationCustomerSupport = 'true'"!]

[!/* This list contains the following and it looks like this:
         0              1                  2                         3                        4
     TimeBaseID : TimeBaseName : NotificationCustomerID : NotificationCustomerName : hasCallbackConfigured ;
     
e.g. StbM has 3 time bases as follows:
     TimeBase_0 contains 3 NotificationCustomers
     TimeBase_1 contains 2 NotificationCustomers
     TimeBase_2 contains 1 NotificationCustomer

     The ListOfTimeBasesWithNotificationCustomers will look like this:
       0 : TimeBase_0 : 0 : TimeBase_0_NotificationCustomer_0 : false
       0 : TimeBase_0 : 1 : TimeBase_0_NotificationCustomer_1 : false
       0 : TimeBase_0 : 2 : TimeBase_0_NotificationCustomer_2 : false
       1 : TimeBase_1 : 0 : TimeBase_1_NotificationCustomer_0 : false
       1 : TimeBase_1 : 1 : TimeBase_1_NotificationCustomer_1 : false
       2 : TimeBase_1 : 0 : TimeBase_2_NotificationCustomer_0 : false
*/!]

[!VAR "ListOfTimeBasesWithNotificationCustomers" = "''"!]

[!// loop over all time bases which have StbMNotificationCustomers
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[count(./StbMNotificationCustomer/*) > 0], './StbMSynchronizedTimeBaseIdentifier')"!]
  [!VAR "timeBaseID" = "node:value(./StbMSynchronizedTimeBaseIdentifier)"!][!//
  [!VAR "timeBaseName" = "node:name(.)"!][!//

  [!// for each time base, loop over all StbMNotificationCustomers
  [!LOOP "node:order(./StbMNotificationCustomer/*, './StbMNotificationCustomerId')"!]
    [!VAR "notificationCustomerID" = "node:value(./StbMNotificationCustomerId)"!][!//
    [!VAR "notificationCustomerName" = "node:name(.)"!][!//
    [!VAR "isCallbackConfigured" = "node:exists(./StbMTimeNotificationCallback)"!]

    [!VAR "ListOfTimeBasesWithNotificationCustomers" = "concat($ListOfTimeBasesWithNotificationCustomers, ' ', $timeBaseID, ':', $timeBaseName, ':', $notificationCustomerID, ':', $notificationCustomerName, ':', $isCallbackConfigured, ';')"!]

  [!ENDLOOP!]
[!ENDLOOP!]


[!ENDIF!]






[!/*
**********************************************************************************************************************
* This macro checks which BusTSyn is linked to the StbM time base, being it a Master or a Slave TimeDomain
**********************************************************************************************************************
*/!]
[!MACRO "GetConnectedBusTSynModule", "timeBase"!][!NOCODE!]

  [!SELECT "node:ref($timeBase)"!]
    [!// take the value of the time base id
    [!VAR "searchedTimeBaseID" = "node:value(./StbMSynchronizedTimeBaseIdentifier) "!]
  [!ENDSELECT!]

  [!// presume that StbM time base is neither Master nor Slave
  [!VAR "isMasterConnected" = "'noMaster'"!]
  [!VAR "isSlaveConnected" = "'noSlave'"!]
  [!VAR "isMasterSlaveConnected" = "'noGateway'"!]
  
  [!VAR "allowSystemWide" = "'false'"!]
  [!VAR "isSystemWide" = "'false'"!]

  [!// presume that there's no connected TSyn bus
  [!VAR "isLinkedToMasterCanTSyn" = "'noCanTSynMaster'"!]
  [!VAR "isLinkedToMasterFrTSyn" = "'noFrTSynMaster'"!]
  [!VAR "isLinkedToMasterEthTSyn" = "'noEthTSynMaster'"!]
  [!VAR "isLinkedToMasterEthTSynPdelayResponder" = "'noEthTSynMasterPdelayResponder'"!]

  [!VAR "isLinkedToSlaveCanTSyn" = "'noCanTSynSlave'"!]
  [!VAR "isLinkedToSlaveFrTSyn" = "'noFrTSynSlave'"!]
  [!VAR "isLinkedToSlaveEthTSyn" = "'noEthTSynSlave'"!]
  [!VAR "isLinkedToSlaveEthTSynPdelayInitiator" = "'noEthTSynSlavePdelayInitiator'"!]

  [!VAR "isTimeValidationEnabledForThisTimeBase" = "false()"!]

  [!// loop over all list of connections
  [!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]

    [!// take the current time base id from the list
    [!VAR "timeBaseIdFromList" = "text:split( ., ':' )[1]"!]

    [!// check if time base id exists in the list of bus connections
    [!IF "num:i($searchedTimeBaseID) = num:i($timeBaseIdFromList)"!]

      [!VAR "isMasterConnected" = "text:split( ., ':' )[2]"!]
      [!VAR "isSlaveConnected" = "text:split( ., ':' )[3]"!]
      [!VAR "isMasterSlaveConnected" = "text:split( ., ':' )[4]"!]
      
      [!VAR "allowSystemWide" = "text:split( ., ':' )[16]"!]
      [!VAR "isSystemWide" = "text:split( ., ':' )[17]"!]

      [!VAR "isLinkedToMasterCanTSyn" = "text:split( ., ':' )[5]"!]
      [!VAR "isLinkedToMasterFrTSyn" = "text:split( ., ':' )[6]"!]
      [!VAR "isLinkedToMasterEthTSyn" = "text:split( ., ':' )[7]"!]
      [!VAR "isLinkedToMasterEthTSynPdelayResponder" = "text:split( ., ':' )[8]"!]

      [!VAR "isLinkedToSlaveCanTSyn" = "text:split( ., ':' )[9]"!]
      [!VAR "isLinkedToSlaveFrTSyn" = "text:split( ., ':' )[10]"!]
      [!VAR "isLinkedToSlaveEthTSyn" = "text:split( ., ':' )[11]"!]
      [!VAR "isLinkedToSlaveEthTSynPdelayInitiator" = "text:split( ., ':' )[12]"!]

      [!VAR "isTimeValidationEnabledForThisTimeBase" = "text:split( ., ':' )[14]"!]

      [!BREAK!]

    [!ENDIF!]

  [!ENDLOOP!]

[!ENDNOCODE!][!ENDMACRO!]



[!// check if master time domains are present
[!VAR "masterTimeDomainsPresent" = "false()"!]
[!// loop over all elements from ListOfBusConnectionsToTimeBase
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]
  [!VAR "masterConnected" = "text:split( ., ':' )[2]"!]
  [!VAR "gatewayConnected" = "text:split( ., ':' )[4]"!]
  [!// if there is at least one master connected
  [!IF "(($masterConnected = 'isMaster') or ($gatewayConnected = 'true'))"!]
    [!VAR "masterTimeDomainsPresent" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]


[!// check if there is at least 1 master time base which has StbMAllowSystemWideGlobalTimeMaster activated
[!VAR "atLeastOneMasterTimeBaseWithAllowSystemWideEnabled" = "false()"!]
[!// loop over all elements from ListOfBusConnectionsToTimeBase
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]
  [!VAR "masterConnected" = "text:split( ., ':' )[2]"!]
  [!VAR "gatewayConnected" = "text:split( ., ':' )[4]"!]
  [!VAR "allowSystemWideEnabled" = "text:split( ., ':' )[19]"!]
  [!// if there is at least one master connected, with StbMAllowSystemWideGlobalTimeMaster set to TRUE
  [!IF "((($masterConnected = 'isMaster') or ($gatewayConnected = 'true')) and ($allowSystemWideEnabled = 'true'))"!]
    [!VAR "atLeastOneMasterTimeBaseWithAllowSystemWideEnabled" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!// check if master time domains are present
[!VAR "masterTimeDomainsPresentWithSystemWideUsed" = "false()"!]
[!// loop over all elements from ListOfBusConnectionsToTimeBase
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]
  [!VAR "masterConnected" = "text:split( ., ':' )[2]"!]
  [!VAR "gatewayConnected" = "text:split( ., ':' )[4]"!]
  [!VAR "allowSystemWideGlobalMaster" = "text:split( ., ':' )[16]"!]
  [!VAR "isSystemWideGlobalMaster" = "text:split( ., ':' )[17]"!]
  [!// if there is at least one master connected, with StbMAllowSystemWideGlobalTimeMaster or StbMIsSystemWideGlobalTimeMaster set to TRUE
  [!IF "((($masterConnected = 'isMaster') or ($gatewayConnected = 'true')) and (($allowSystemWideGlobalMaster = 'true') or ($isSystemWideGlobalMaster = 'true')))"!]
    [!VAR "masterTimeDomainsPresentWithSystemWideUsed" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!// check if master offset time domains are present
[!VAR "masterOffsetTimeDomainsPresentWithSystemWideUsed" = "false()"!]
[!// loop over all elements from ListOfBusConnectionsToTimeBase
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]
  [!VAR "masterConnected" = "text:split( ., ':' )[2]"!]
  [!VAR "gatewayConnected" = "text:split( ., ':' )[4]"!]
  [!VAR "allowSystemWideGlobalMaster" = "text:split( ., ':' )[16]"!]
  [!VAR "isSystemWideGlobalMaster" = "text:split( ., ':' )[17]"!]
  [!VAR "tmpTimeBaseId" = "text:split( ., ':' )[1]"!]
  [!// if there is at least one offset master connected, with StbMAllowSystemWideGlobalTimeMaster or StbMIsSystemWideGlobalTimeMaster set to TRUE
  [!IF "((($masterConnected = 'isMaster') or ($gatewayConnected = 'true')) and (($allowSystemWideGlobalMaster = 'true') or ($isSystemWideGlobalMaster = 'true')) and (($tmpTimeBaseId > 15) and ($tmpTimeBaseId < 32 )))"!]
    [!VAR "masterOffsetTimeDomainsPresentWithSystemWideUsed" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!// check if there is at least 1 master time base which has StbMAllowMasterRateCorrection activated
[!VAR "atLeastOneMasterTimeBaseWithAllowRateCorrectionActivated" = "false()"!]
[!// loop over all elements from ListOfBusConnectionsToTimeBase
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]
  [!VAR "masterConnected" = "text:split( ., ':' )[2]"!]
  [!VAR "allowMasterRateCorrectionActivatedOnTimeBase" = "text:split( ., ':' )[21]"!]
  [!// if there is at least one master connected, with StbMAllowMasterRateCorrection set to TRUE
  [!IF "(($masterConnected = 'isMaster') and ($allowMasterRateCorrectionActivatedOnTimeBase = 'true'))"!]
    [!VAR "atLeastOneMasterTimeBaseWithAllowRateCorrectionActivated" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]


[!// check if slave time domains are present
[!VAR "slaveTimeDomainsPresent" = "false()"!]
[!// loop over all elements from ListOfBusConnectionsToTimeBase
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!]
  [!VAR "SlaveConnected" = "text:split( ., ':' )[3]"!]
  [!VAR "GatewayConnected" = "text:split( ., ':' )[4]"!]
  [!// if there is at least one slave or gateway connected
  [!IF "(($SlaveConnected = 'isSlave') or ($GatewayConnected = 'true'))"!]
    [!VAR "slaveTimeDomainsPresent" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]






[!IF "(node:value(as:modconf('StbM')[1]/StbMGeneral/StbMTimeRecordingSupport) = 'true')"!]

[!VAR "atLeastOneSyncSlaveStbMTimeBaseWithTimeRecording" = "false()"!]
[!// loop through all StbM time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// numberOfSyncTableBlocks are supported only on Slave time bases, because StbMTimeRecording can now be activated also on Master time bases
  [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and ((node:exists(StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)) and (node:value(StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) > 0)))"!]
    [!VAR "atLeastOneSyncSlaveStbMTimeBaseWithTimeRecording" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!VAR "atLeastOneOffsetSlaveStbMTimeBaseWithTimeRecording" = "false()"!]
[!// loop through all StbM Offset time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[node:exists(StbMOffsetTimeBase)]"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// numberOfOffsetTableBlocks are supported only on Slave time bases, because StbMTimeRecording can now be activated also on Master time bases
  [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and ((node:exists(StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount)) and (node:value(StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount) > 0)))"!]
    [!VAR "atLeastOneOffsetSlaveStbMTimeBaseWithTimeRecording" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!ENDIF!]



[!IF "($StbMTimeValidationUsed = 'true')"!]


[!// check if there is an EthTSyn Time Domain which is referencing an StbM Time Base which has TimeValidation enabled
[!VAR "atLeastOneStbMTimeBaseReferedByEthTSynMasterOrSlaveWhenTimeValidationEnabled" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "ethMasterConnected" = "text:split( ., ':' )[7]"!][!//
[!VAR "ethSlaveConnected" = "text:split( ., ':' )[11]"!][!//
[!VAR "isTimeValidationEnabledOnTimeBase" = "text:split( ., ':' )[14]"!][!//
[!IF "((($ethMasterConnected = 'masterEthTSyn') or ($ethSlaveConnected = 'slaveEthTSyn')) and ($isTimeValidationEnabledOnTimeBase = 'true'))"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByEthTSynMasterOrSlaveWhenTimeValidationEnabled" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]

[!// check if there is a MASTER FrTSyn Time Domain which is referencing at least one StbM Time Base
[!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInMasterListTimeValidation" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "frMasterConnected" = "text:split( ., ':' )[6]"!][!//
[!VAR "isTimeValidationActiveOnFrMaster" = "text:split( ., ':' )[14]"!][!//
[!IF "($frMasterConnected = 'masterFrTSyn') and ($isTimeValidationActiveOnFrMaster = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInMasterListTimeValidation" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]

[!// check if there is a SLAVE FrTSyn Time Domain which is referencing at least one StbM Time Base
[!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInSlaveListTimeValidation" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "frSlaveConnected" = "text:split( ., ':' )[10]"!][!//
[!VAR "isTimeValidationActiveOnFrSlave" = "text:split( ., ':' )[14]"!][!//
[!IF "($frSlaveConnected = 'slaveFrTSyn') and ($isTimeValidationActiveOnFrSlave = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInSlaveListTimeValidation" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]



[!// check if there is a MASTER CanTSyn Time Domain which is referencing at least one StbM Time Base
[!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInMasterListTimeValidation" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "canMasterConnected" = "text:split( ., ':' )[5]"!][!//
[!VAR "isTimeValidationActiveOnCanMaster" = "text:split( ., ':' )[14]"!][!//
[!IF "($canMasterConnected = 'masterCanTSyn') and ($isTimeValidationActiveOnCanMaster = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInMasterListTimeValidation" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]

[!// check if there is a SLAVE CanTSyn Time Domain which is referencing at least one StbM Time Base
[!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInSlaveListTimeValidation" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "canSlaveConnected" = "text:split( ., ':' )[9]"!][!//
[!VAR "isTimeValidationActiveOnCanSlave" = "text:split( ., ':' )[14]"!][!//
[!IF "($canSlaveConnected = 'slaveCanTSyn') and ($isTimeValidationActiveOnCanSlave = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInSlaveListTimeValidation" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]


[!ENDIF!]


[!IF "node:exists(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold)"!]

[!VAR "atLeastOneStbMNotificationCustomerWithCallbackDisabled" = "false()"!]
[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[count(./StbMNotificationCustomer/*) > 0], './StbMSynchronizedTimeBaseIdentifier')"!]
  [!LOOP "node:order(./StbMNotificationCustomer/*, './StbMNotificationCustomerId')"!]
    [!IF "(not(node:exists(./StbMTimeNotificationCallback)))"!]
      [!VAR "atLeastOneStbMNotificationCustomerWithCallbackDisabled" = "true()"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDLOOP!]
[!ENDLOOP!]

[!ENDIF!]


[!// - EOF ----------------------
[!ENDSELECT!]
[!ENDIF!]
[!ENDNOCODE!]
