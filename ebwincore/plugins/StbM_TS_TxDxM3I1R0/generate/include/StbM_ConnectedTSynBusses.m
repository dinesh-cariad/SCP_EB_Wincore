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
[!IF "not(var:defined('STBM_CONNECTEDTSYNBUSSES_M'))"!]
[!VAR "STBM_CONNECTEDTSYNBUSSES_M"="'true'"!]



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


[!SELECT "as:modconf('StbM')[1]"!]

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
      1          2                    3               4               5                      6                    7                               8                             9                    10                   11                           12                           13                             14                             15                                  16                                      17                                       18                19                               20                                     21
     ID : isMasterConnected : isSlaveConnected : isGateway : isMasterCANConnected : isMasterFRConnected : isMasterETHConnected : isMasterETHPDelayResponderConnected : isSlaveCANConnected : isSlaveFRConnected : isSlaveETHConnected : isSlaveETHPDelayInitiatorConnected : stbMTimeBaseName :            isTimeValidationEnabled  :    isTimeRecordingEnabled     :    allowSystemWideGlobalMasterForMaster  :   isSystemWideGlobalMasterForMaster  :  isTimeRecordingUsed  :    isAllowSystemWideUsed  :   isStatusNotificationUsedOnTimeBase   :   isAllowMasterRateCorrectionActivatedOnTimeBase
e.g. 14       noMaster               noSlave        true         masterCanTSyn         noFrTSynMaster        noEthTSynMaster          noEthTSynPdelayResponder             noCanTSynSlave          slaveFrTSyn        noEthTSynSlave          noEthTSynPdelayInitiator       StbM_SynchronizedTimeBase_4              true                          true                           false                                     false                                  false                false                         false                                    false
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


[!// - EOF ----------------------
[!ENDSELECT!]
[!ENDIF!]
[!ENDNOCODE!]
