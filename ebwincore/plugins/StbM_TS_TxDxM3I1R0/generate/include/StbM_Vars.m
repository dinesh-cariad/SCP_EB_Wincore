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
[!IF "not(var:defined('STBM_VARS_M'))"!]
[!VAR "STBM_VARS_M"="'true'"!]

[!INCLUDE "StbM_ConnectedTSynBusses.m"!]

[!SELECT "as:modconf('StbM')[1]"!]


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
  
  [!VAR "isTimeRecordingEnabled" = "'false'"!]

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
  [!VAR "isAllowMasterRateCorrectionActivatedForThisTimeBase" = "false()"!]

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
      [!VAR "isAllowMasterRateCorrectionActivatedForThisTimeBase" = "text:split( ., ':' )[21]"!]
      
      [!VAR "isTimeRecordingEnabled" = "text:split( ., ':' )[15]"!]
      
      [!BREAK!]

    [!ENDIF!]

  [!ENDLOOP!]

[!ENDNOCODE!][!ENDMACRO!]



=== Get Rate Correction / Offset Correction calculation interval as time stamp type ===
[!MACRO "GetTimeIntervalRate", "timeBaseNode"!][!NOCODE!]
[!SELECT "node:ref($timeBaseNode)"!]

  [!IF "node:exists(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) and (node:value(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) != 0)"!]
    [!IF "node:exists(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration)"!]
      [!VAR "Interval" = "(node:value(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) div node:value(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration))"!]
    [!ELSE!]
      [!VAR "Interval" = "node:value(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration)"!]
    [!ENDIF!]
    [!VAR "rateCorrNs" = "num:i(num:f(($Interval - floor($Interval)) * num:i(1000000000)))"!]
    [!VAR "rateCorrSec" = "num:i(floor($Interval))"!]

    [!VAR "doubleTheRateCorrSec" = "num:i(2*num:i($rateCorrSec))"!]
    [!/* !LINKSTO StbM.EB.SWS_StbM_00367.Allow10ms,1 */!]
    [!// add 10ms to compensate for delays in BuSetGlobalTime calls
    [!VAR "doubleTheRateCorrNs" = "num:i(2*num:i($rateCorrNs) + num:i(10000000))"!]
    [!IF "($doubleTheRateCorrNs >= num:i(1000000000))"!]
      [!VAR "doubleTheRateCorrNs" = "num:i($doubleTheRateCorrNs - num:i(1000000000))"!]
      [!VAR "doubleTheRateCorrSec" = "num:i($doubleTheRateCorrSec + 1)"!]
    [!ENDIF!]
  [!ELSE!]
    [!VAR "rateCorrNs" = "num:i(0)"!]
    [!VAR "rateCorrSec" = "num:i(0)"!]
    [!VAR "doubleTheRateCorrNs" = "num:i(0)"!]
    [!VAR "doubleTheRateCorrSec" = "num:i(0)"!]
  [!ENDIF!]

[!ENDSELECT!]
[!ENDNOCODE!][!ENDMACRO!]

[!MACRO "GetTimeIntervalOffset", "timeBaseNode"!][!NOCODE!]
[!SELECT "node:ref($timeBaseNode)"!]

    [!VAR "offsetCorrAdaptNs" = "num:i(0)"!]
    [!VAR "offsetCorrAdaptSec" = "num:i(0)"!]
    [!VAR "offsetCorrAdaptNsLo" = "num:i(0)"!]
    [!VAR "offsetCorrAdaptNsHi" = "num:i(0)"!]
    [!VAR "offsetCorrJumpNs" = "num:i(0)"!]
    [!VAR "offsetCorrJumpSec" = "num:i(0)"!]

  [!IF "node:exists(./StbMTimeCorrection/StbMOffsetCorrectionJumpThreshold) and (node:value(./StbMTimeCorrection/StbMOffsetCorrectionJumpThreshold) != 0)"!]
    [!VAR "adaptationInterval" = "node:value(./StbMTimeCorrection/StbMOffsetCorrectionAdaptionInterval)"!]
    [!VAR "offsetCorrAdaptNs" = "num:i(num:f(($adaptationInterval - floor($adaptationInterval)) * num:i(1000000000)))"!]
    [!VAR "offsetCorrAdaptSec" = "num:i(floor($adaptationInterval))"!]

    [!// convert adaption interval to Virtual Local Time type
    [!IF "num:i((num:f($adaptationInterval) * num:f(1000000000))) > num:i(4294967295)"!]
      [!VAR "offsetCorrAdaptNsLo" = "num:i((num:f($adaptationInterval) * num:f(1000000000)) - num:f(4294967295))"!]
      [!VAR "offsetCorrAdaptNsHi" = "num:i(1)"!]
    [!ELSE!]
      [!VAR "offsetCorrAdaptNsLo" = "num:i((num:f($adaptationInterval) * num:f(1000000000)))"!]
      [!VAR "offsetCorrAdaptNsHi" = "num:i(0)"!]
    [!ENDIF!]

    [!VAR "offsetJumpThreshold" = "node:value(./StbMTimeCorrection/StbMOffsetCorrectionJumpThreshold)"!]
    [!VAR "offsetCorrJumpNs" = "num:i(num:f(($offsetJumpThreshold - floor($offsetJumpThreshold)) * num:i(1000000000)))"!]
    [!VAR "offsetCorrJumpSec" = "num:i(floor($offsetJumpThreshold))"!]
  [!ELSE!]
    [!VAR "offsetCorrAdaptNs" = "num:i(0)"!]
    [!VAR "offsetCorrAdaptSec" = "num:i(0)"!]
    [!VAR "offsetCorrAdaptNsLo" = "num:i(0)"!]
    [!VAR "offsetCorrAdaptNsHi" = "num:i(0)"!]
    [!VAR "offsetCorrJumpNs" = "num:i(0)"!]
    [!VAR "offsetCorrJumpSec" = "num:i(0)"!]
  [!ENDIF!]

[!ENDSELECT!]
[!ENDNOCODE!][!ENDMACRO!]



=== Get the name of the SyncTimeRecord, OffsetTimeRecord and NotificationStatus CALLBACK ===
[!MACRO "GetTimeBaseCallbackName", "timeBaseNode", "functionName"!][!NOCODE!]
[!SELECT "node:ref($timeBaseNode)"!]
  [!VAR "timeBaseName" = "node:name(.)"!]
  [!VAR "callbackName" = "concat($functionName, $timeBaseName)"!]
[!ENDSELECT!]
[!ENDNOCODE!][!ENDMACRO!]


=== Get the name of the TimeNotification CALLBACK ===
[!MACRO "GetTimeNotificationCallbackName", "customer"!][!NOCODE!]
[!SELECT "node:ref($customer)"!]
  [!VAR "timeBaseName" = "node:name(./../..)"!]
  [!VAR "customerName" = "node:name(.)"!]
  [!VAR "configuredName" = "node:value(StbMTimeNotificationCallback)"!]
  [!VAR "timeNotificationCallbackName" = "concat($customerName,'_',$configuredName,$timeBaseName)"!]
[!ENDSELECT!]
[!ENDNOCODE!][!ENDMACRO!]



[!/*
****************************************************************************************************
* Frequencies for which a more precise conversion macro is defined in StbM_TimeConversionMacros.h file
****************************************************************************************************
*/!]
[!VAR "MostCommonlyUsedFrequenciesList" = "''"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '10000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '16000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '1001675')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '2000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '2080000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '3000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '3250000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '3500000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '4000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '5000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '6250000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '8000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '10000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '12500000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '15000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '16000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '20000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '24000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '24550000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '25000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '26000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '32000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '33300000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '37500000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '40000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '45000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '48000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '50000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '55000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '56000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '60000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '62500000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '64000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '70000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '75000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '80000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '90000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '96000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '100000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '110000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '120000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '132000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '133000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '140000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '125000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '150000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '160000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '180000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '200000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '240000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '250000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '264000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '300000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '330000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '400000000')"!]
[!VAR "MostCommonlyUsedFrequenciesList" = "concat($MostCommonlyUsedFrequenciesList, ' ', '800000000')"!]

[!/*
**********************************************************************************************************************
* This macro checks if, for an StbM frequency there is or not a macro like this StbM_TicksToNs_[frequencyInHz] defined
* in StbM_TimeConversionMacros.h file.
* If YES, StbM_TicksToNs_[frequencyInHz] shall be used to convert Gpt ticks into nanoseconds, because it is more precise
* If NO, STBM_GPT_CONV_TICKS_TO_NS macro shall be used to convert Gpt ticks into nanoseconds
*
**********************************************************************************************************************
*/!]
[!MACRO "GetNeededFrequency", "timeBase"!][!NOCODE!]
  [!SELECT "node:ref($timeBase)"!]

  [!// compute the factor StbMClockFrequency divided by StbMClockPrescaler
  [!VAR "freq" = "num:i(floor((node:value(./StbMLocalTimeClock/StbMClockFrequency)) div (node:value(./StbMLocalTimeClock/StbMClockPrescaler))))"!]
  [!VAR "neededFrequency" = "num:i(0)"!]
  [!// presume that freq is not present in the most commonly used frequencies list
  [!VAR "isConversionMacroDefinedForFreq" = "false()"!]

  [!// loop over all defined macros, for most commonly used frequencies
  [!LOOP "text:split($MostCommonlyUsedFrequenciesList)"!]

    [!/* !LINKSTO StbM.dsn.Conversion.When.TimeSource.Is.GPT.MacrosFrom.StbM_TimeConversionMacros.Used,1 */!]
    [!// check if freq frequency is found in MostCommonlyUsedFrequenciesList
    [!IF "$freq = ."!]
      [!VAR "isConversionMacroDefinedForFreq" = "true()"!]
      [!VAR "neededFrequency" = "."!]
      [!BREAK!]
    [!ENDIF!]

  [!ENDLOOP!]

  [!ENDSELECT!]
[!ENDNOCODE!][!ENDMACRO!]



[!/*
**********************************************************************************************************************
* This macro checks if, for a Triggered Customer frequency there is or not a macro like this StbM_NsToTicks_[frequencyInHz] defined
* in StbM_TimeConversionMacros.h file.
* If YES, StbM_NsToTocks_[frequencyInHz] shall be used to convert nanoseconds into OS Ticks, because it is more precise
* If NO, formula  nanoseconds * [StbMScheduleTicksToNsConvFactor] / OS_TICKS2NS_[StbMOsCounterId]([StbMScheduleTicksToNsConvFactor])  shall be used to convert nanoseconds into OS Ticks
*
**********************************************************************************************************************
*/!]
[!MACRO "GetTriggeredCustomerNeededFrequency", "triggeredCustomer"!][!NOCODE!]
  [!SELECT "node:ref($triggeredCustomer)"!]

    [!// initialize triggered customer frequency 
    [!VAR "neededTriggeredCustomerFrequency" = "num:i(0)"!]
    [!// presume that freq is not present in the most commonly used frequencies list
    [!VAR "isConversionMacroDefinedForTriggeredCustomerFreq" = "false()"!]
    
    [!IF "node:exists(./StbMScheduleTableClockFreq)"!]
    
      [!// get StbMScheduleTableClockFreq frequency
      [!VAR "freq" = "num:i(floor(node:value(./StbMScheduleTableClockFreq)))"!]
      
      [!// loop over all defined macros, for most commonly used frequencies
      [!LOOP "text:split($MostCommonlyUsedFrequenciesList)"!]
        [!/* !LINKSTO StbM.dsn.Conversion.When.TimeSource.Is.GPT.MacrosFrom.StbM_TimeConversionMacros.Used,1 */!]
        [!// check if freq frequency is found in MostCommonlyUsedFrequenciesList
        [!IF "$freq = ."!]
          [!VAR "isConversionMacroDefinedForTriggeredCustomerFreq" = "true()"!]
          [!VAR "neededTriggeredCustomerFrequency" = "."!]
          [!BREAK!]
        [!ENDIF!]
      [!ENDLOOP!]
      
    [!ENDIF!]
  
  [!ENDSELECT!]
[!ENDNOCODE!][!ENDMACRO!]

[!/*
--------------------------------------------------------------------------
    Set the values of useful variables
--------------------------------------------------------------------------
*/!]

[!// if there is at least one reference to Os --> enable Os counter
[!// if there is at least one reference to EthTSyn --> enable EthTSyn virtual local time
[!// if there is at least one reference to Gpt --> enable Gpt virtual local time
[!// is there is at least one StbM synchronized time base, which has StbMUseOSGetTimeStamp enabled --> enable the usage of OSGetTimeStamp API
[!// count number of time bases that use EthTSyn as virtual local time source
[!VAR "StbMOsCounterUsage" = "false()"!]
[!VAR "StbMGptUsage" = "false()"!]
[!VAR "StbMEthTSynHwCounterUsage" = "false()"!]
[!VAR "atLeastOneTimeBaseWithStbMUseOSGetTimeStampEnableExists" = "false()"!]
[!VAR "nrGptTimeBases" = "num:i(0)"!]
[!LOOP "StbMSynchronizedTimeBase/*[not(node:exists(./StbMOffsetTimeBase))]"!]

[!IF "node:value(./StbMUseOSGetTimeStamp) = 'false'"!]

[!IF "node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../OsCounter)"!]
[!VAR "StbMOsCounterUsage" = "true()"!]
[!ELSEIF "node:exists(node:ref(./StbMLocalTimeClock/StbMLocalTimeHardware)/../../GptChannelConfiguration)"!]
[!VAR "StbMGptUsage" = "true()"!]
[!VAR "nrGptTimeBases" = "num:i($nrGptTimeBases + 1)"!]
[!ELSE!]
[!VAR "StbMEthTSynHwCounterUsage" = "true()"!]
[!ENDIF!]

[!ELSE!]

[!VAR "atLeastOneTimeBaseWithStbMUseOSGetTimeStampEnableExists" = "true()"!]

[!ENDIF!]

[!ENDLOOP!]

[!// at least one time base needs to share data --> enable data sharing
[!VAR "StbMShareDataTimeBasesNumber" = "num:i(0)"!]
[!LOOP "StbMSynchronizedTimeBase/*"!]
[!IF "node:value(./StbMProvideDataInSharedMemory) = 'true'"!]
[!VAR "StbMShareDataTimeBasesNumber" = "num:i($StbMShareDataTimeBasesNumber + 1)"!]
[!ENDIF!]
[!ENDLOOP!]

[!// at least one timeout value used --> enable timeout handling
[!IF "count(StbMSynchronizedTimeBase/*[node:exists(StbMSyncLossTimeout)]) > 0"!]
[!VAR "StbMTimeoutUsage" = "true()"!]
[!ELSE!]
[!VAR "StbMTimeoutUsage" = "false()"!]
[!ENDIF!]

[!// at least one StbMTimeLeapFutureThreshold or StbMTimeLeapPastThreshold value is used --> enable timeleap detection
[!IF "(count(StbMSynchronizedTimeBase/*[node:exists(StbMTimeLeapFutureThreshold)]) > 0) or (count(StbMSynchronizedTimeBase/*[node:exists(StbMTimeLeapPastThreshold)]) > 0)"!]
[!VAR "StbMTimeleapDetection" = "true()"!]
[!ELSE!]
[!VAR "StbMTimeleapDetection" = "false()"!]
[!ENDIF!]

[!// Number of synchronized time bases in StbM configuration
[!VAR "StbMSyncTimebaseNumber" = "num:i(count(StbMSynchronizedTimeBase/*[(not(node:exists(StbMOffsetTimeBase))) and (node:value(StbMSynchronizedTimeBaseIdentifier) < 16)]))"!]
[!// Number of offset time bases in StbM configuration
[!VAR "StbMOffsetTimebaseNumber" = "num:i(count(StbMSynchronizedTimeBase/*[node:exists(StbMOffsetTimeBase)]))"!]
[!// Number of pure time bases in StbM configuration
[!VAR "StbMPureTimebaseNumber" = "num:i(count(StbMSynchronizedTimeBase/*[(not(node:exists(StbMOffsetTimeBase))) and (node:value(StbMSynchronizedTimeBaseIdentifier) >= 32)]))"!]

[!// Number of triggered custumers in StbM configuration
[!VAR "StbMTriggeredCustomersNumber" = "num:i(count(StbMTriggeredCustomer/*))"!]




[!/* Number of modules configured as connected BSW modules.
     If Os counter are used we treat the Os counter as one this connected modules.
     This allows to abstract function calls in the static code.*/!]
[!VAR "StbMBswModulesNumber" = "num:i(0)"!]
[!VAR "noOfModulesInBswModulesList" = "num:i(count(as:modconf('StbM')[1]/StbMBswModules/*))"!]
[!VAR "noSetModuleInList" = "false()"!]
[!IF "$noOfModulesInBswModulesList = num:i(0)"!]
[!VAR "noSetModuleInList" = "true()"!]
[!VAR "StbMBswModulesNumber" = "num:i(1)"!]
[!ELSE!]
[!LOOP "as:modconf('StbM')[1]/StbMBswModules/*"!]
  [!VAR "BswModuleName" = "node:name(.)"!]
  [!VAR "key" = "concat($BswModuleName, '.AdjacentLayerConfig.ModuleConfigName')"!]
  [!IF "ecu:has($key)"!]
    [!VAR "StbMBswModulesNumber" = "num:i($StbMBswModulesNumber + 1)"!][!//
  [!ENDIF!]
  [!VAR "key" = "concat($BswModuleName, '.AdjacentLayerConfig.ApiName.V1.SetGlobalTime')"!]
  [!IF "not(ecu:has($key))"!]
    [!VAR "noSetModuleInList" = "true()"!]
  [!ENDIF!]
[!ENDLOOP!]
[!IF "($noSetModuleInList = 'true')"!][!//
  [!VAR "StbMBswModulesNumber" = "num:i($StbMBswModulesNumber + 1)"!][!//
[!ENDIF!][!//
[!ENDIF!]

[!// Store if the module EthTSyn is present in our configuration as lower layer
[!VAR "StbMEthTSynPresent" = "false()"!]
[!LOOP "StbMBswModules/*"!]
  [!IF "node:name(.) = 'EthTSyn'"!]
    [!VAR "StbMEthTSynPresent" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]



[!// at least one SYNC TimeBase has StbMTimeValidation enabled --> enable Time Validation feature
[!VAR "StbMTimeValidationUsed" = "false()"!]
[!// loop through all SYNC StbM time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(node:value(./StbMSynchronizedTimeBaseIdentifier) < 16)]"!]
  [!IF "(node:exists(./StbMTimeValidation))"!]
    [!VAR "StbMTimeValidationUsed" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]


[!/* - if there is at least one time base that has Master Rate Correction activated --> enable Master Rate Correction
     - if there is at least one time base that has Offset Correction activated --> enable Offset Correction
     - if there is at least one time base that has Offset Correction activated --> enable Offset Correction
 - count the number of StbMRateCorrectionsPerMeasurementDuration -> how many members the
 StbM_SlaveRateCorrectionStartValues[] array will have
 - count the number of Sync and Offset Blocks for Time Recording
 - count the number of Time Bases that use Rate Correction -> how many members the
 StbM_SlaveRateCorrectionData[] array will have */!]

[!VAR "StbMRateCorrectionUsage" = "false()"!]
[!VAR "StbMOffsetCorrectionUsage" = "false()"!]
[!VAR "StbMMasterRateCorrectionUsage" = "false()"!]
[!VAR "StbMNvMUsage" = "false()"!]
[!VAR "StbMGetMasterConfigSupported" = "false()"!]
[!VAR "numberOfRateCorrectionsPerAllMeasurements" = "num:i(0)"!]
[!VAR "numberOfSlaveRateCorrectionTimeBases" = "num:i(0)"!]
[!VAR "numberOfMasterRateCorrectionTimeBases" = "num:i(0)"!]
[!VAR "numberOfSyncTableBlocks" = "num:i(0)"!]
[!VAR "numberOfOffsetTableBlocks" = "num:i(0)"!]

[!IF "($StbMTimeValidationUsed = 'true')"!]
[!VAR "numberOfSyncTimeValidationTableBlocks" = "num:i(0)"!]
[!ENDIF!]

[!LOOP "StbMSynchronizedTimeBase/*"!]
  [!IF "node:exists(./StbMTimeCorrection/StbMAllowMasterRateCorrection) and (node:value(./StbMTimeCorrection/StbMAllowMasterRateCorrection) = 'true')"!]
    [!VAR "StbMMasterRateCorrectionUsage" = "true()"!]
    [!VAR "numberOfMasterRateCorrectionTimeBases" = "num:i($numberOfMasterRateCorrectionTimeBases + 1)"!]
  [!ELSE!]
     <!-- !LINKSTO StbM.ASR44.SWS_StbM_00400,1 -->
     [!IF "node:exists(./StbMTimeCorrection/StbMOffsetCorrectionJumpThreshold) and (node:value(./StbMTimeCorrection/StbMOffsetCorrectionJumpThreshold) != 0)"!]
       [!VAR "StbMOffsetCorrectionUsage" = "true()"!]
     [!ENDIF!]

     <!-- !LINKSTO StbM.ASR44.SWS_StbM_00377,1 -->
     [!IF "node:exists(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) and (node:value(./StbMTimeCorrection/StbMRateCorrectionMeasurementDuration) != 0)"!]
       [!IF "node:exists(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration)"!]
         [!VAR "numberOfRateCorrectionsPerAllMeasurements" = "$numberOfRateCorrectionsPerAllMeasurements + num:i(node:value(./StbMTimeCorrection/StbMRateCorrectionsPerMeasurementDuration))"!]
       [!ELSE!]
         [!VAR "numberOfRateCorrectionsPerAllMeasurements" = "num:i($numberOfRateCorrectionsPerAllMeasurements + 1)"!]
       [!ENDIF!]
       [!VAR "numberOfSlaveRateCorrectionTimeBases" = "num:i($numberOfSlaveRateCorrectionTimeBases + 1)"!]
     [!VAR "StbMRateCorrectionUsage" = "true()"!]
     [!ENDIF!]


     [!IF "node:exists(./StbMTimeRecording)"!]

       [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]

       [!// numberOfSyncTableBlocks and numberOfOffsetTableBlocks are supported only on Slave and gateway time bases
       [!IF "(($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true'))"!]
         [!VAR "numberOfSyncTableBlocks" = "num:i($numberOfSyncTableBlocks + node:value(./StbMTimeRecording/StbMSyncTimeRecordTableBlockCount))"!]
         [!VAR "numberOfOffsetTableBlocks" = "num:i($numberOfOffsetTableBlocks + node:value(./StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount))"!]
       [!ENDIF!]

       [!IF "($StbMTimeValidationUsed = 'true')"!]
         [!IF "(node:exists(./StbMTimeValidation))"!]
           [!VAR "numberOfSyncTimeValidationTableBlocks" = "num:i($numberOfSyncTimeValidationTableBlocks + node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount))"!]
         [!ENDIF!]
       [!ENDIF!]

     [!ENDIF!]


  [!ENDIF!]

  [!IF "((node:exists(./StbMStoreTimebaseNonVolatile)) and (node:value(./StbMStoreTimebaseNonVolatile) = 'STORAGE_AT_SHUTDOWN'))"!]
    [!VAR "StbMNvMUsage" = "true()"!]
  [!ENDIF!]
  
  
  [!IF "(node:value(./StbMSynchronizedTimeBaseIdentifier) < 31)"!]
  
    [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
    
    [!// Check if SYNC or OFFSET time base is a master one
    [!IF "(($isMasterConnected = 'isMaster') or ($isMasterSlaveConnected = 'true'))"!]
      [!IF "(node:exists(./StbMAllowSystemWideGlobalTimeMaster) = 'true')"!]
        [!VAR "StbMGetMasterConfigSupported" = "true()"!]
      [!ENDIF!]
    [!ENDIF!]
	
  [!ELSE!]

      [!// PURE time bases are considered masters
      [!IF "(node:exists(./StbMAllowSystemWideGlobalTimeMaster) = 'true')"!]
        [!VAR "StbMGetMasterConfigSupported" = "true()"!]
      [!ENDIF!]
  
  [!ENDIF!]

[!ENDLOOP!]




[!VAR "atLeastOneMasterOrGatewayTimeBase" = "false()"!]
[!// loop through all StbM SYNC and OFFSET time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// check if there is at least one sync/offset master
  [!IF "(($isMasterConnected = 'isMaster') or ($isMasterSlaveConnected = 'true'))"!]
    [!VAR "atLeastOneMasterOrGatewayTimeBase" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!VAR "atLeastOneSyncOrOffsetMasterTimeBase" = "false()"!]
[!// loop through all StbM SYNC and OFFSET time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(node:value(./StbMSynchronizedTimeBaseIdentifier) <= 31)]"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// check if there is at least one sync/offset master
  [!IF "(($isMasterConnected = 'isMaster') or ($isMasterSlaveConnected = 'true'))"!]
    [!VAR "atLeastOneSyncOrOffsetMasterTimeBase" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]

[!VAR "atLeastOneOffsetMasterOrGatewayTimeBase" = "false()"!]
[!// loop through all StbM OFFSET time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(node:exists(./StbMOffsetTimeBase))]"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// check if there is at least one offset master or one offset gateway time base
  [!IF "(($isMasterConnected = 'isMaster') or ($isMasterSlaveConnected = 'true'))"!]
    [!VAR "atLeastOneOffsetMasterOrGatewayTimeBase" = "true()"!]
    [!BREAK!]
  [!ENDIF!]
[!ENDLOOP!]




[!VAR "StbMTimeBaseSyncTimeRecord" = "false()"!]
[!VAR "isSyncTimeRecordBlockCallbackUsed" = "false()"!]
[!// loop through all StbM time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// numberOfSyncTableBlocks are supported only on Slave time bases, because StbMTimeRecording can now be activated also on Master time bases
  [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and ($isSystemWide = 'false') and ((node:exists(StbMTimeRecording/StbMSyncTimeRecordTableBlockCount)) and (node:value(StbMTimeRecording/StbMSyncTimeRecordTableBlockCount) > 0)))"!]
    [!VAR "StbMTimeBaseSyncTimeRecord" = "true()"!]
    [!// check if there is at least 1 callback configured on a SYNC SLAVE/GATEWAY time base 
    [!IF "(node:exists(StbMTimeRecording/StbMSyncTimeRecordBlockCallback))"!]
      [!VAR "isSyncTimeRecordBlockCallbackUsed" = "true()"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]

[!VAR "StbMTimeBaseSyncSlaveExists" = "false()"!]
[!// loop through all StbM time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!IF "(($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true'))"!]
    [!VAR "StbMTimeBaseSyncSlaveExists" = "true()"!]
  [!ENDIF!]
[!ENDLOOP!]

[!VAR "StbMTimeBaseOffsetTimeRecord" = "false()"!]
[!VAR "isOffsetTimeRecordBlockCallbackUsed" = "false()"!]
[!// loop through all StbM Offset time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[node:exists(StbMOffsetTimeBase)]"!]
  [!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]
  [!// numberOfOffsetTableBlocks are supported only on Slave time bases, because StbMTimeRecording can now be activated also on Master time bases
  [!IF "((($isSlaveConnected = 'isSlave') or ($isMasterSlaveConnected = 'true')) and ($isSystemWide = 'false') and ((node:exists(StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount)) and (node:value(StbMTimeRecording/StbMOffsetTimeRecordTableBlockCount) > 0)))"!]
    [!VAR "StbMTimeBaseOffsetTimeRecord" = "true()"!]
    [!// check if there is at least 1 callback configured on a OFFSET SLAVE/GATEWAY time base 
    [!IF "(node:exists(StbMTimeRecording/StbMOffsetTimeRecordBlockCallback))"!]
      [!VAR "isOffsetTimeRecordBlockCallbackUsed" = "true()"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]



[!// at least one TimeBase has StbMNotificationInterface configured to CALLBACK, SR_INTERFACE or CALLBACK_AND_SR_INTERFACE --> enable notification status feature
[!VAR "StbMTimeBaseNotificationStatus" = "false()"!]
[!// check if at least 1 callback is used for STATUS Notification
[!VAR "isStatusNotificationCallbackUsed" = "false()"!]
[!// loop through all StbM time bases
[!LOOP "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*"!]
  [!IF "((node:exists(StbMNotificationInterface)) and (node:value(StbMNotificationInterface) != 'NO_NOTIFICATION'))"!]
    [!VAR "StbMTimeBaseNotificationStatus" = "true()"!]
    [!// check if there exists at least 1 Time Base, which has StbMNotificationInterface configured to CALLBACK or CALLBACK_AND_SR_INTERFACE
    [!IF "((node:value(StbMNotificationInterface) = 'CALLBACK') or (node:value(StbMNotificationInterface) = 'CALLBACK_AND_SR_INTERFACE'))"!]
      [!VAR "isStatusNotificationCallbackUsed" = "true()"!]
      [!BREAK!]
    [!ENDIF!]
  [!ENDIF!]
[!ENDLOOP!]


[!// Number of notification customers in StbM configuration
[!VAR "StbMNotificationCustomersNumber" = "num:i(0)"!]
[!// check if at least 1 callback is used for TIME Notification
[!VAR "isTimeNotificationCallbackUsed" = "false()"!]
[!// loop through all StbM time bases which have at least one child in StbMNotificationCustomer List
[!LOOP "StbMSynchronizedTimeBase/*[(count(StbMNotificationCustomer/*) > 0)]"!]
  [!// loop through all StbMNotificationCustomers
  [!LOOP "StbMNotificationCustomer/*"!]
    [!IF "(node:exists(StbMTimeNotificationCallback))"!]
      [!VAR "isTimeNotificationCallbackUsed" = "true()"!]
    [!ENDIF!]
    [!// increase StbMNotificationCustomersNumber while iterating
    [!VAR "StbMNotificationCustomersNumber" = "num:i($StbMNotificationCustomersNumber + 1)"!]
  [!ENDLOOP!]
[!ENDLOOP!]


[!VAR "isStbMExternalsHeaderFileNeeded" = "false()"!]
[!IF "(($isSyncTimeRecordBlockCallbackUsed = 'true') or ($isOffsetTimeRecordBlockCallbackUsed = 'true') or ($isStatusNotificationCallbackUsed = 'true') or ($isTimeNotificationCallbackUsed = 'true'))"!]
  [!VAR "isStbMExternalsHeaderFileNeeded" = "true()"!]
[!ENDIF!]



[!// check if StbMRteUsage parameter is activated
[!VAR "StbMRteUsed" = "node:value(as:modconf('StbM')[1]/StbMGeneral/StbMRteUsage)"!]

[!// check if StbMTimeRecordingSupport parameter is activated
[!VAR "StbMTimeRecordingUsed" = "node:value(as:modconf('StbM')[1]/StbMGeneral/StbMTimeRecordingSupport)"!]




[!IF "($StbMTimeValidationUsed = 'true')"!]

[!/*
******************************************************************************************************************************************
* Checking if there is at least one StbM TimeBase which has an EthTSyn Time Domain connected and it has Time Validation enabled
******************************************************************************************************************************************
*/!]
[!VAR "atLeastOneStbMTimeBaseReferedByEthTSynWithTimeValidationEnabled" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "ethMasterConnected" = "text:split( ., ':' )[7]"!][!//
[!VAR "ethSlaveConnected" = "text:split( ., ':' )[11]"!][!//
[!VAR "isTimeValidationActiveOnTimeBase" = "text:split( ., ':' )[14]"!][!//
[!IF "((($ethMasterConnected = 'masterEthTSyn') or ($ethSlaveConnected = 'slaveEthTSyn')) and ($isTimeValidationActiveOnTimeBase = 'true'))"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByEthTSynWithTimeValidationEnabled" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]



[!/*
****************************************************************************************************************************************
* Checking if there is at least one StbM TimeBase which has a MASTER FrTSyn Time Domain connected and it has Time Validation enabled
****************************************************************************************************************************************
*/!]
[!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInMasterList" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "frMasterConnected" = "text:split( ., ':' )[6]"!][!//
[!VAR "isTimeValidationActiveOnFrMaster" = "text:split( ., ':' )[14]"!][!//
[!IF "($frMasterConnected = 'masterFrTSyn') and ($isTimeValidationActiveOnFrMaster = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInMasterList" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]

[!/*
****************************************************************************************************************************************
* Checking if there is at least one StbM TimeBase which has a SLAVE FrTSyn Time Domain connected and it has Time Validation enabled
***************************************************************************************************************************************
*/!]
[!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInSlaveList" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "frSlaveConnected" = "text:split( ., ':' )[10]"!][!//
[!VAR "isTimeValidationActiveOnFrSlave" = "text:split( ., ':' )[14]"!][!//
[!IF "($frSlaveConnected = 'slaveFrTSyn') and ($isTimeValidationActiveOnFrSlave = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByFrTSynInSlaveList" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]




[!/*
*****************************************************************************************************************************************
* Checking if there is at least one StbM TimeBase which has a MASTER CanTSyn Time Domain connected and it has Time Validation enabled
*****************************************************************************************************************************************
*/!]
[!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInMasterList" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "canMasterConnected" = "text:split( ., ':' )[5]"!][!//
[!VAR "isTimeValidationActiveOnCanMaster" = "text:split( ., ':' )[14]"!][!//
[!IF "($canMasterConnected = 'masterCanTSyn') and ($isTimeValidationActiveOnCanMaster = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInMasterList" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]

[!/*
****************************************************************************************************************************************
* Checking if there is at least one StbM TimeBase which has a SLAVE CanTSyn Time Domain connected and it has Time Validation enabled
****************************************************************************************************************************************
*/!]
[!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInSlaveList" = "false()"!]
[!LOOP "text:split($ListOfBusConnectionsToTimeBase, ';')"!][!//
[!VAR "canSlaveConnected" = "text:split( ., ':' )[9]"!][!//
[!VAR "isTimeValidationActiveOnCanSlave" = "text:split( ., ':' )[14]"!][!//
[!IF "($canSlaveConnected = 'slaveCanTSyn') and ($isTimeValidationActiveOnCanSlave = 'true')"!][!//
  [!VAR "atLeastOneStbMTimeBaseReferedByCanTSynInSlaveList" = "true()"!]
  [!BREAK!]
[!ENDIF!]
[!ENDLOOP!]




[!/*
***********************************************************************************************************
* Counting how many StbM time bases which can record CANTSYN/FRTSYN/ETHTSYN Master/Slave timing data exist
***********************************************************************************************************
*/!]
[!VAR "numberOfCanTSynMasterTimingDataBlocks" = "num:i(0)"!]
[!VAR "numberOfFrTSynMasterTimingDataBlocks" = "num:i(0)"!]
[!VAR "numberOfCanTSynSlaveTimingDataBlocks" = "num:i(0)"!]
[!VAR "numberOfFrTSynSlaveTimingDataBlocks" = "num:i(0)"!]
[!VAR "numberOfEthTSynMasterOrSlaveTimingDataBlocks" = "num:i(0)"!]

[!LOOP "node:order(as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*[(node:value(StbMSynchronizedTimeBaseIdentifier) < 16)], './StbMSynchronizedTimeBaseIdentifier')"!]
[!CALL "GetConnectedBusTSynModule", "timeBase" = "node:path(.)"!]


[!IF "(node:exists(./StbMTimeValidation))"!]

[!IF "($isLinkedToMasterCanTSyn = 'masterCanTSyn')"!]
  [!VAR "numberOfCanTSynMasterTimingDataBlocks" = "num:i($numberOfCanTSynMasterTimingDataBlocks + node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount))"!]
[!ENDIF!]

[!IF "($isLinkedToMasterFrTSyn = 'masterFrTSyn')"!]
  [!VAR "numberOfFrTSynMasterTimingDataBlocks" = "num:i($numberOfFrTSynMasterTimingDataBlocks + node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount))"!]
[!ENDIF!]


[!IF "($isLinkedToSlaveCanTSyn = 'slaveCanTSyn')"!]
  [!VAR "numberOfCanTSynSlaveTimingDataBlocks" = "num:i($numberOfCanTSynSlaveTimingDataBlocks + node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount))"!]
[!ENDIF!]

[!IF "($isLinkedToSlaveFrTSyn = 'slaveFrTSyn')"!]
  [!VAR "numberOfFrTSynSlaveTimingDataBlocks" = "num:i($numberOfFrTSynSlaveTimingDataBlocks + node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount))"!]
[!ENDIF!]


[!IF "(($isLinkedToMasterEthTSyn = 'masterEthTSyn') or ($isLinkedToSlaveEthTSyn = 'slaveEthTSyn'))"!]
  [!VAR "numberOfEthTSynMasterOrSlaveTimingDataBlocks" = "num:i($numberOfEthTSynMasterOrSlaveTimingDataBlocks + node:value(./StbMTimeValidation/StbMTimeValidationRecordTableBlockCount))"!]
[!ENDIF!]

[!ENDIF!]

[!ENDLOOP!]


[!ENDIF!]



[!// check if StbMTimerStartThreshold parameter is activated
[!VAR "StbMNotificationOfCustomersUsed" = "node:exists(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold)"!]


[!IF "node:exists(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold)"!]
[!VAR "maxValueSec1" = "4294967296"!]
[!VAR "integerPartSec1" = "num:i(node:value(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold))"!]
[!VAR "floatPartSec1" = "node:value(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartThreshold) - $integerPartSec1"!]

[!VAR "timerStartThresholdNs" = "num:i($floatPartSec1 * 1000000000)"!]
[!VAR "timerStartThresholdSec" = "num:i($integerPartSec1 mod $maxValueSec1)"!]
[!ENDIF!]




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








[!// - EOF ----------------------
[!ENDSELECT!]
[!ENDIF!]
[!ENDNOCODE!]
