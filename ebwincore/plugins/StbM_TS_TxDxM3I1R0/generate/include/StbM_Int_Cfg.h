[!AUTOSPACING!]
#ifndef STBM_CFG_INT_H
#define STBM_CFG_INT_H

/**
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
 */
/** \addtogroup Synchronized Time Base Manager
 ** @{ */

[!INCLUDE "StbM_Vars.m"!]
/*==[Includes]================================================================*/

#include <StbM_Int_Stc.h>
#include <StbM_Gen_Common.h>

[!IF "(($StbMGptUsage = 'true') or ($StbMNotificationOfCustomersUsed = 'true'))"!]
/* include Gpt header for type Gpt_ChannelType only if there is at least one refence pointing to Gpt. */
/* !LINKSTO StbM.ASR44.SWS_StbM_00426,1 */
#include <Gpt.h>
[!ENDIF!]

/* !LINKSTO StbM.ASR44.SWS_StbM_00065,1 */
/* !LINKSTO StbM.EB.StbMUseOSGetTimeStamp.Enabled.OsInclusion.Needed,1 */
[!IF "(($StbMTriggeredCustomersNumber > 0) or ($StbMOsCounterUsage = 'true') or ($atLeastOneTimeBaseWithStbMUseOSGetTimeStampEnableExists = 'true'))"!]
/* !LINKSTO StbM.dsn.OsInclusion,1 */
#include <StbM_Int_Os.h>
[!ENDIF!]

/* include Rte_StbM_Type.h file before the StbM_Types.h file inclusion,
   in order to assure that re-definitions of StbM types in case Non-EB Rte is used, will not happen */
#if (STBM_RTE_USED == STD_ON)
[!LOOP "text:split($ListOfEcucPartitionNames, ';')"!]
[!VAR "EcuCPartitionName" = "."!]
[!IF "$EcuCPartitionName = '_NoPartition'"!]
  [!VAR "EcuCPartitionName" = "''"!]
[!ENDIF!]
#include <Rte_StbM[!"$EcuCPartitionName"!]_Type.h>
[!ENDLOOP!]
#endif /* STBM_RTE_USED == STD_ON */

/*==[Macros]==================================================================*/

#if (defined STBM_DEV_ERROR_DETECT)
#error STBM_DEV_ERROR_DETECT is already defined.
#endif
/** \brief Switch for enabling the development error detection. */
#define STBM_DEV_ERROR_DETECT                     STD_[!IF "StbMGeneral/StbMDevErrorDetect"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_TIMEOUTS_USED)
#error STBM_TIMEOUTS_USED is already defined.
#endif
/** \brief Enables/disables timeout handling. */
#define STBM_TIMEOUTS_USED                        STD_[!IF "$StbMTimeoutUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_TIMELEAP_DETECTION_USED)
#error STBM_TIMELEAP_DETECTION_USED is already defined.
#endif
/** \brief Enables/disables timeleap detection. */
#define STBM_TIMELEAP_DETECTION_USED              STD_[!IF "$StbMTimeleapDetection = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_STATUS_NOTIFICATIONS_USED)
#error STBM_STATUS_NOTIFICATIONS_USED is already defined.
#endif
/** \brief Enables/disables status notifications for certain time bases. */
#define STBM_STATUS_NOTIFICATIONS_USED            STD_[!IF "$StbMTimeBaseNotificationStatus = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]




#if (defined STBM_TRIGGERED_CUSTOMERS_USED)
#error STBM_TRIGGERED_CUSTOMERS_USED is already defined.
#endif
/** \brief Whether there are OS customers */
#define STBM_TRIGGERED_CUSTOMERS_USED             STD_[!IF "$StbMTriggeredCustomersNumber > 0"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_NUMBER_OF_TRIGGERED_CUSTOMERS)
#error STBM_NUMBER_OF_TRIGGERED_CUSTOMERS is already defined.
#endif
/** \brief Number of configured triggered customer. */
#define STBM_NUMBER_OF_TRIGGERED_CUSTOMERS        [!"$StbMTriggeredCustomersNumber"!]U



#if (defined STBM_NUMBER_OF_NOTIFICATION_CUSTOMERS)
#error STBM_NUMBER_OF_NOTIFICATION_CUSTOMERS is already defined.
#endif
/** \brief Number of configured notification customers. */
#define STBM_NUMBER_OF_NOTIFICATION_CUSTOMERS        [!"$StbMNotificationCustomersNumber"!]U



#if (defined STBM_OS_COUNTER_USED)
#error STBM_OS_COUNTER_USED is already defined.
#endif
/** \brief Enables/disables access to Os counters. */
#define STBM_OS_COUNTER_USED                      STD_[!IF "$StbMOsCounterUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED)
#error STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED is already defined.
#endif
/** \brief Enables/disables access to the freerunning HW counter from the corresponding Ethernet Controller via EthIf_GetCurrentTime() */
#define STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED       STD_[!IF "$StbMEthTSynHwCounterUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_GPT_USED)
#error STBM_GPT_USED is already defined.
#endif
/** \brief Enables/disables access to Gpt timers. */
#define STBM_GPT_USED       STD_[!IF "$StbMGptUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_OSGETTIMESTAMP_USED)
#error STBM_OSGETTIMESTAMP_USED is already defined.
#endif
/** \brief Enables/disables access to OSGetTimeStamp API */
#define STBM_OSGETTIMESTAMP_USED       STD_[!IF "$atLeastOneTimeBaseWithStbMUseOSGetTimeStampEnableExists = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_SLAVE_RATE_CORRECTION_USED)
#error STBM_SLAVE_RATE_CORRECTION_USED is already defined.
#endif
/** \brief Enables/disables Slave Rate Correction calculation. */
#define STBM_SLAVE_RATE_CORRECTION_USED           STD_[!IF "$StbMRateCorrectionUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_OFFSET_CORRECTION_USED)
#error STBM_OFFSET_CORRECTION_USED is already defined.
#endif
/** \brief Enables/disables Rate Correction calculation. */
#define STBM_OFFSET_CORRECTION_USED               STD_[!IF "$StbMOffsetCorrectionUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_OFFSET_TIMEBASES_USED)
#error STBM_OFFSET_TIMEBASES_USED is already defined.
#endif
/** \brief Whether there are offset timebases configured */
#define STBM_OFFSET_TIMEBASES_USED                STD_[!IF "$StbMOffsetTimebaseNumber > 0"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_PURE_TIMEBASES_USED)
#error STBM_PURE_TIMEBASES_USED is already defined.
#endif
/** \brief Whether there are offset timebases configured */
#define STBM_PURE_TIMEBASES_USED                STD_[!IF "$StbMPureTimebaseNumber > 0"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_NUMBER_OF_ADJACENT_MODULES)
#error STBM_NUMBER_OF_ADJACENT_MODULES is already defined.
#endif
/** \brief Number of adjacent modules as configured in StbMBswModules
 ** plus the Os if counters are used */
#define STBM_NUMBER_OF_ADJACENT_MODULES           [!"$StbMBswModulesNumber"!]U

#if (defined STBM_NUMBER_TIME_BASES_THAT_SHARE_DATA_MODULES)
#error STBM_NUMBER_TIME_BASES_THAT_SHARE_DATA_MODULES is already defined.
#endif
/** \brief Number of time bases that share data to a memory location
 ** plus the Os if counters are used */
#define STBM_NUMBER_TIME_BASES_THAT_SHARE_DATA_MODULES           [!"$StbMShareDataTimeBasesNumber"!]U

#if (defined STBM_TIME_BASES_THAT_SHARE_DATA_USED)
#error STBM_TIME_BASES_THAT_SHARE_DATA_USED is already defined.
#endif
/** \brief Whether there are timebases that share memory data configured */
#define STBM_TIME_BASES_THAT_SHARE_DATA_USED                STD_[!IF "$StbMShareDataTimeBasesNumber > 0"!]ON[!ELSE!]OFF[!ENDIF!]

[!IF "$StbMRateCorrectionUsage = 'true'"!]
#if (defined STBM_NUMBER_OF_RATE_CORRECTION_INTERVALS)
#error STBM_NUMBER_OF_RATE_CORRECTION_INTERVALS is already defined.
#endif
/** \brief Number of saved time stamps needed to cover all intervals
 ** of Rate Correction calculations */
#define STBM_NUMBER_OF_RATE_CORRECTION_INTERVALS  [!"num:i($numberOfRateCorrectionsPerAllMeasurements)"!]U

#if (defined STBM_NUMBER_OF_SLAVE_RATE_CORRECTION_TIMEBASES)
#error STBM_NUMBER_OF_SLAVE_RATE_CORRECTION_TIMEBASES is already defined.
#endif
/** \brief Number of time bases that use Rate Correction */
#define STBM_NUMBER_OF_SLAVE_RATE_CORRECTION_TIMEBASES  [!"num:i($numberOfSlaveRateCorrectionTimeBases)"!]U
[!ENDIF!]

[!IF "$StbMMasterRateCorrectionUsage = 'true'"!]
#if (defined STBM_NUMBER_OF_MASTER_RATE_CORRECTION_TIMEBASES)
#error STBM_NUMBER_OF_MASTER_RATE_CORRECTION_TIMEBASES is already defined.
#endif
/** \brief Number of time bases that use Rate Correction */
#define STBM_NUMBER_OF_MASTER_RATE_CORRECTION_TIMEBASES  [!"num:i($numberOfMasterRateCorrectionTimeBases)"!]U
[!ENDIF!]

[!IF "$StbMTimeBaseSyncTimeRecord = 'true'"!]
#if (defined STBM_NUMBER_OF_SYNC_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases Time Recording */
#define STBM_NUMBER_OF_SYNC_TIME_RECORD_BLOCKS  [!"num:i($numberOfSyncTableBlocks)"!]U
[!ENDIF!]



[!IF "($StbMTimeValidationUsed = 'true')"!][!//


#if (defined STBM_NUMBER_OF_SYNC_TIME_VALIDATION_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_TIME_VALIDATION_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases Time Recording which have TimeValidation enabled */
#define STBM_NUMBER_OF_SYNC_TIME_VALIDATION_TIME_RECORD_BLOCKS  [!"num:i($numberOfSyncTimeValidationTableBlocks)"!]U


#if (defined STBM_NUMBER_OF_SYNC_CANTSYN_MASTER_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_CANTSYN_MASTER_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases which are linked to Master CanTSyn time domains */
#define STBM_NUMBER_OF_SYNC_CANTSYN_MASTER_TIME_RECORD_BLOCKS  [!"num:i($numberOfCanTSynMasterTimingDataBlocks)"!]U

#if (defined STBM_NUMBER_OF_SYNC_FRTSYN_MASTER_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_FRTSYN_MASTER_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases which are linked to Master FrTSyn time domains */
#define STBM_NUMBER_OF_SYNC_FRTSYN_MASTER_TIME_RECORD_BLOCKS  [!"num:i($numberOfFrTSynMasterTimingDataBlocks)"!]U


#if (defined STBM_NUMBER_OF_SYNC_CANTSYN_SLAVE_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_CANTSYN_SLAVE_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases which are linked to Slave CanTSyn time domains */
#define STBM_NUMBER_OF_SYNC_CANTSYN_SLAVE_TIME_RECORD_BLOCKS  [!"num:i($numberOfCanTSynSlaveTimingDataBlocks)"!]U

#if (defined STBM_NUMBER_OF_SYNC_FRTSYN_SLAVE_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_FRTSYN_SLAVE_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases which are linked to Slave FrTSyn time domains */
#define STBM_NUMBER_OF_SYNC_FRTSYN_SLAVE_TIME_RECORD_BLOCKS  [!"num:i($numberOfFrTSynSlaveTimingDataBlocks)"!]U


#if (defined STBM_NUMBER_OF_SYNC_ETHTSYN_MASTER_OR_SLAVE_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_SYNC_ETHTSYN_MASTER_OR_SLAVE_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Sync Time Bases which are linked to Master EthTSyn time domains */
#define STBM_NUMBER_OF_SYNC_ETHTSYN_MASTER_OR_SLAVE_TIME_RECORD_BLOCKS  [!"num:i($numberOfEthTSynMasterOrSlaveTimingDataBlocks)"!]U

[!ENDIF!]



[!IF "$StbMTimeBaseOffsetTimeRecord = 'true'"!]
#if (defined STBM_NUMBER_OF_OFFSET_TIME_RECORD_BLOCKS)
#error STBM_NUMBER_OF_OFFSET_TIME_RECORD_BLOCKS is already defined.
#endif
/** \brief Number of blocks for Offset Time Bases Time Recording */
#define STBM_NUMBER_OF_OFFSET_TIME_RECORD_BLOCKS  [!"num:i($numberOfOffsetTableBlocks)"!]U
[!ENDIF!]

#if (defined STBM_NUMBER_OF_GPT_TIMEBASES)
#error STBM_NUMBER_OF_GPT_TIMEBASES is already defined.
#endif
/** \brief Number of Time Bases that use Gpt as a time reference */
#define STBM_NUMBER_OF_GPT_TIMEBASES       [!"$nrGptTimeBases"!]U

[!IF "$StbMOsCounterUsage = 'true'"!]
[!VAR "StbMOsCounter" = "as:modconf('StbM')[1]/StbMSynchronizedTimeBase/*/StbMLocalTimeClock/StbMLocalTimeHardware[node:exists(node:ref(.)/../../OsCounter)]"!]
[!VAR "StbMOsCounterId" = "node:name(node:ref($StbMOsCounter))"!]
#if (defined STBM_OS_COUNTER_ID)
#error STBM_OS_COUNTER_ID is already defined.
#endif
/** \brief Name of the referenced Os counter (= symbolic name for the counter Id) */
#define STBM_OS_COUNTER_ID                        [!"$StbMOsCounterId"!]

[!VAR "StbMOsCounterMaxValue" = "node:value((node:ref($StbMOsCounter))/OsCounterMaxAllowedValue)"!]
#if (defined STBM_MAX_OS_COUNTER_VALUE_IN_TICKS)
#error STBM_MAX_OS_COUNTER_VALUE_IN_TICKS is already defined.
#endif
/** \brief Name of the referenced Os counter (= symbolic name for the counter Id) */
#define STBM_MAX_OS_COUNTER_VALUE_IN_TICKS        [!"$StbMOsCounterMaxValue"!]U

#if (defined STBM_OS_COUNTER_CONV_TICKS_TO_NS)
#error STBM_OS_COUNTER_CONV_TICKS_TO_NS is already defined.
#endif
/** \brief This macro converts ticks of the Os Counter used as StbMLocalTimeHardware into nanoseconds */
/* !LINKSTO StbM.EB.OsCounter.Used.OSTicks.To.Ns.Conversion,1 */
#define STBM_OS_COUNTER_CONV_TICKS_TO_NS(x)       (OS_TICKS2NS_[!"$StbMOsCounterId"!]((x)))
[!ENDIF!]

#if (defined STBM_UPDATE_COUNTER_VALUE)
#error STBM_UPDATE_COUNTER_VALUE is already defined.
#endif
/** \brief A 3 secconds counter based on the StbM Main Function */
#define STBM_UPDATE_COUNTER_VALUE  [!"num:i(floor(3 div node:value(StbMGeneral/StbMMainFunctionPeriod)))"!]



[!IF "($StbMTimeValidationUsed = 'true')"!][!//

#if (defined STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED)
#error STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED is already defined.
#endif
/** \brief Enables/disables Time Validation interfaces and interface's types
           which are used by EthTSyn module to provide master timing recording data to StbM */
#define STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED        STD_[!IF "$atLeastOneStbMTimeBaseReferedByEthTSynWithTimeValidationEnabled = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED)
#error STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED is already defined.
#endif
/** \brief Enables/disables Time Validation interfaces and interface's types
           which are used by FrTSyn module to provide master timing recording data to StbM */
#define STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED              STD_[!IF "$atLeastOneStbMTimeBaseReferedByFrTSynInMasterList = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED)
#error STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED is already defined.
#endif
/** \brief Enables/disables Time Validation interfaces and interface's types
           which are used by FrTSyn module to provide slave timing recording data to StbM */
#define STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED              STD_[!IF "$atLeastOneStbMTimeBaseReferedByFrTSynInSlaveList = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]



#if (defined STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED)
#error STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED is already defined.
#endif
/** \brief Enables/disables Time Validation interfaces and interface's types
           which are used by CanTSyn module to provide master timing recording data to StbM */
#define STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED              STD_[!IF "$atLeastOneStbMTimeBaseReferedByCanTSynInMasterList = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED)
#error STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED is already defined.
#endif
/** \brief Enables/disables Time Validation interfaces and interface's types
           which are used by CanTSyn module to provide slave timing recording data to StbM */
#define STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED              STD_[!IF "$atLeastOneStbMTimeBaseReferedByCanTSynInSlaveList = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

[!ENDIF!]


#define STBM_COMMON_FEATURES_ENABLED                                                                                                           \
((STBM_TIMEOUTS_USED == STD_ON) || (STBM_STATUS_NOTIFICATIONS_USED == STD_ON) ||                                                               \
(STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_TIMELEAP_DETECTION_USED == STD_ON) ||                                                     \
(STBM_SLAVE_RATE_CORRECTION_USED == STD_ON) || ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)) ||   \
(STBM_TIME_VALIDATION_SUPPORT == STD_ON) || (STBM_MASTER_RATE_CORRECTION_USED == STD_ON) ||                                                    \
(STBM_NVM_USED == STD_ON) || (STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON)  || (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON) ||           \
(STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON))


[!IF "$StbMNotificationOfCustomersUsed = 'true'"!]

 #if (defined STBM_TIMERSTARTTHRESHOLD_NANOSEC_PART)
 #error STBM_TIMERSTARTTHRESHOLD_NANOSEC_PART is already defined.
 #endif
 /** \brief  This macro represents the StbMTimerStartThreshold value */
 #define STBM_TIMERSTARTTHRESHOLD_NANOSEC_PART  [!"$timerStartThresholdNs"!]U

#if (defined STBM_TIMERSTARTTHRESHOLD_SEC_PART)
 #error STBM_TIMERSTARTTHRESHOLD_SEC_PART is already defined.
 #endif
 /** \brief  This macro represents the StbMTimerStartThreshold value */
 #define STBM_TIMERSTARTTHRESHOLD_SEC_PART  [!"$timerStartThresholdSec"!]U


#if (defined STBM_MAX_NUMBER_OF_STBM_STARTTIMER_CALLS)
 #error STBM_MAX_NUMBER_OF_STBM_STARTTIMER_CALLS is already defined.
 #endif
 /** \brief  This macro represents the maximum number of StbM_StartTimer() calls */
 #define STBM_MAX_NUMBER_OF_STBM_STARTTIMER_CALLS  [!"node:value(as:modconf('StbM')[1]/StbMGeneral/StbMTimerStartMaxNumberOfCalls)"!]U


#if (defined STBM_GPT_CHANNEL_USED_FOR_STARTING_GPTTIMER)
 #error STBM_GPT_CHANNEL_USED_FOR_STARTING_GPTTIMER is already defined.
 #endif
 /** \brief  This macro represents the Gpt channel used for notification of customers */
 #define STBM_GPT_CHANNEL_USED_FOR_STARTING_GPTTIMER  [!"node:value(node:ref(as:modconf('StbM')[1]/StbMGeneral/StbMGptTimerRef)/GptChannelId)"!]U

[!ENDIF!]



/* ----- defensive programming macros ----- */



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "StbMDefensiveProgramming"!][!//

#if (defined STBM_DEFENSIVE_PROGRAMMING_ENABLED)
#error STBM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define STBM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../StbMGeneral/StbMDevErrorDetect  = 'true') and (StbMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined STBM_PRECONDITION_ASSERT_ENABLED)
#error STBM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define STBM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../StbMGeneral/StbMDevErrorDetect  = 'true') and (StbMDefProgEnabled = 'true') and (StbMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined STBM_POSTCONDITION_ASSERT_ENABLED)
#error STBM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define STBM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../StbMGeneral/StbMDevErrorDetect  = 'true') and (StbMDefProgEnabled = 'true') and (StbMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined STBM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error STBM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define STBM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../StbMGeneral/StbMDevErrorDetect  = 'true') and (StbMDefProgEnabled = 'true') and (StbMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined STBM_INVARIANT_ASSERT_ENABLED)
#error STBM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define STBM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../StbMGeneral/StbMDevErrorDetect  = 'true') and (StbMDefProgEnabled = 'true') and (StbMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined STBM_STATIC_ASSERT_ENABLED)
#error STBM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define STBM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../StbMGeneral/StbMDevErrorDetect  = 'true') and (StbMDefProgEnabled = 'true') and (StbMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//


/*==[Types]===================================================================*/

#if (STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON)

/* !LINKSTO StbM.EB.SharedMemorySupport.StbM_EthOffsetType,1 */
/** \brief This typedef defines the type of the Shared Memory of one time base
 */
typedef struct
{
  VAR(StbM_TimeStampType, TYPEDEF) OffsetTime;  /**<  current Offset time */
  VAR(StbM_RateDeviationType, TYPEDEF) RateDeviation;  /**< current rate correction value, always 1 if not used */
  VAR(uint8, TYPEDEF) TimeBaseId;  /**< time base id as per StbM configuration */
  VAR(uint8, TYPEDEF) OffsetSign;  /**< determines if the Offset should be added or subtracted from the HwCounter */
  VAR(uint8, TYPEDEF) WriteBeginCnt;  /**< is incremented before to start update of Offset* members */
  VAR(uint8, TYPEDEF) WriteEndCnt;  /**< is incremented after finishing the update of Offset* members */
} StbM_EthOffsetType;

/* !LINKSTO StbM.EB.SharedMemorySupport.StbM_SharedEthOffsetType,1 */
/** \brief This typedef defines the type of the Shared Memory of all time bases
 */
typedef struct
{
  VAR(uint32, TYPEDEF) Version;  /**< Current Version of this type; Currently it is always 1 */
  VAR(uint32, TYPEDEF) TimeBasesNumber;  /**< Number of time bases (offset array elements) */
  VAR(StbM_EthOffsetType, TYPEDEF) Offset[STBM_NUMBER_TIME_BASES_THAT_SHARE_DATA_MODULES];  /**<  Current Offset Time Data */
} StbM_SharedEthOffsetType;

#endif /* STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON */



#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)

/** \brief Represent a short type Time Tuple
 **
 ** Variables of this type store a Time Tuple containing the Global Time and
 ** the associated Virtual Local Time (only seconds, without secondsHi).
 */
typedef struct
{
  VAR(StbM_TimeStampShortType, TYPEDEF) timeValidationGlobalTime;  /**< Global Time */
  VAR(StbM_VirtualLocalTimeType, TYPEDEF) timeValidationVirtualLocalTime;  /**< Associated Virtual Local Time */
} StbM_TimeValidationTimeTupleType;



/** \brief Time Recording information
 **
 ** This type contains runtime information regarding
 ** time recording
 */
typedef struct
{
  VAR(uint32, TYPEDEF) counter;  /**< block counter */
  VAR(boolean, TYPEDEF) wasCounterWrapped;  /**< let the next StbM_<Bus>SetMasterTimingData() or StbM_<Bus>SetSlaveTimingData() API, know that the counter was wrapped */
} StbM_TimeValidationInfoType;


#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */


/** \brief Function pointer to time provider
 **
 ** This type defines a function pointer, pointing to the API service
 ** for obtaining the global time.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_TimeGetterFctPtrType)
(
  VAR(StbM_SynchronizedTimeBaseType, STBM_VAR) timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) currentTimeTupleGlobalPtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) currentTimeTupleVLTPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);

/** \brief Function pointer to time provider
 **
 ** This type defines a function pointer, pointing to the API service
 ** for obtaining the Virtual Local Time.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_VirtualLocalTimeGetterFctPtrType)
(
  VAR(StbM_SynchronizedTimeBaseType, STBM_VAR) timeBaseId,
  P2VAR( StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);


/** \brief Synchronized time base config
 **
 ** This type contains the precompile time configuration
 ** of a synchronized time base
 */
typedef struct
{
  CONST(StbM_VirtualLocalTimeGetterFctPtrType, TYPEDEF) virtualLocalTimeGetterFctPtr;  /**< where to obtain the Virtual Local Time */
  CONST(StbM_TimeGetterFctPtrType, TYPEDEF) timeGetterFctPtr;  /**< where to obain the time */
  CONSTP2CONST(uint8, TYPEDEF, STBM_APPL_CONST) timeSetterIndexPtr;  /**< points to index list to identify the connected masters */
  CONST(uint8, TYPEDEF) numberOfTimeSetter;  /**< Number of connected master time domains */
  CONST(uint8, TYPEDEF) timeBaseLocalTimeIdx;  /**< information about Local Hardware time provenance (if time is derived from EthIf, this represents the Ethernet Controller index)*/
  CONST(uint8, TYPEDEF) role;  /**< Indicates role of time base (master, slave or gateway) */
} StbM_SyncTimeBaseCfgType;

/** \brief Offset time base config
 **
 ** This type contains the precompile time configuration
 ** of an offset time base
 */
typedef struct
{
  CONST(uint8, TYPEDEF) syncTimeBaseIndex;  /**< Index of the related synchronized time base */
} StbM_OffsetTimeBaseCfgType;


#if (STBM_GPT_USED == STD_ON)
/** \brief Gpt time bases config
 **
 ** This type contains the precompile time configuration
 ** of the gptChannelId of the time bases that have the Gpt as virtual time source
 */
typedef struct
{
  CONST(Gpt_ValueType, TYPEDEF) gptMaxChannelTickValue;  /**< Max Channel Tick Value of the related Gpt Channel */
  CONST(Gpt_ChannelType, TYPEDEF) gptChannelId;  /**< Channel Id of the related Gpt Channel */
} StbM_GptTimeBaseCfgType;
#endif /* (STBM_GPT_USED == STD_ON) */


#if (STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED == STD_ON)
/** \brief EthTSyn time bases config
 **
 ** This type contains the precompile time configuration
 ** of the EthIf controller indexes referenced by EthTSynGlobalTimeDomains which are referred by StbM time bases
 */
typedef struct
{
  CONST(uint8, TYPEDEF) ethIfControllerIdx;  /**< EthIf controller index */
} StbM_EthTSynTimeBaseCfgType;
#endif /* (STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED == STD_ON) */


/** \brief Common time base config
 **
 ** This type contains the precompile time configuration
 ** of a time base (sync and offset)
 */
#if (STBM_COMMON_FEATURES_ENABLED)
typedef struct
{
#if (STBM_TIMEOUTS_USED == STD_ON)
  CONST(uint32, TYPEDEF) timeoutValue;  /**< timeout of time base in MainFunction invocations */
#endif /* STBM_TIMEOUTS_USED == STD_ON */
#if (STBM_STATUS_NOTIFICATIONS_USED == STD_ON)
  CONST(uint32, TYPEDEF) statusNotificationMask;  /**< Configured notification mask for the enabled status bits */
#endif /* STBM_STATUS_NOTIFICATIONS_USED == STD_ON */
#if ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON))
  CONST(uint32, TYPEDEF) startIndexTrBlocks;  /**< start index in the array where time recording values are stored */
#endif /* ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON)) */
#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
  CONST(uint32, TYPEDEF) startIndexTrBlocksTimeValidationMASTERPart;  /**< startIndexTrBlocksTimeValidationMASTERPart */
  CONST(uint32, TYPEDEF) startIndexTrBlocksTimeValidationMASTERPdelayPart;  /**< startIndexTrBlocksTimeValidationMASTERPdelayPart */
  CONST(uint32, TYPEDEF) startIndexTrBlocksTimeValidationSLAVEPart;  /**< startIndexTrBlocksTimeValidationSLAVEPart */
  CONST(uint32, TYPEDEF) startIndexTrBlocksTimeValidationSLAVEPdelayPart;  /**< startIndexTrBlocksTimeValidationSLAVEPdelayPart */
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */
#if (STBM_TIMELEAP_DETECTION_USED == STD_ON)
  CONST(uint32, TYPEDEF) timeleapFutureValueNs;  /**< timeleap future value - nanoseconds part */
  CONST(uint32, TYPEDEF) timeleapFutureValueSec;  /**< timeleap future value - seconds part */
  CONST(uint32, TYPEDEF) timeleapPastValueNs;  /**< timeleap past value - nanoseconds part */
  CONST(uint32, TYPEDEF) timeleapPastValueSec;  /**< timeleap past value - seconds part */
  CONST(uint16, TYPEDEF) clearTimeleapCount;  /**< clear time leap counter */
#endif /* STBM_TIMELEAP_DETECTION_USED == STD_ON */
#if (STBM_OFFSET_CORRECTION_USED == STD_ON)
  CONST(uint32, TYPEDEF) offsetCorrectionAdaptIntervalNs;  /**< offset correction adaptation interval - nanoseconds part */
  CONST(uint8, TYPEDEF) offsetCorrectionAdaptIntervalSec;  /**< offset correction adaptation interval - seconds part */
  CONST(uint32, TYPEDEF) offsetCorrectionAdaptIntervalNsLo;  /**< offset correction adaptation interval in VLT type - nanosecondsLo part */
  CONST(uint8, TYPEDEF) offsetCorrectionAdaptIntervalNsHi;  /**< offset correction adaptation interval in VLT type - nanosecondsHi part */
  CONST(uint32, TYPEDEF) offsetCorrectionJumpThresholdNs;  /**< offset correction calculation interval - nanoseconds part */
  CONST(uint8, TYPEDEF) offsetCorrectionJumpThresholdSec;  /**< offset correction calculation interval - seconds part */
#endif /* STBM_OFFSET_CORRECTION_USED == STD_ON */
#if (STBM_SLAVE_RATE_CORRECTION_USED == STD_ON)
  CONST(uint32, TYPEDEF) rateCorrectionIntervalNs;  /**< rate correction calculation interval - nanoseconds part */
  CONST(uint16, TYPEDEF) rateCorrectionIntervalSec;  /**< rate correction calculation interval - seconds part */
  CONST(uint32, TYPEDEF) doubleTheRateCorrectionIntervalNs;  /**< double the rate correction calculation interval - nanoseconds part */
  CONST(uint16, TYPEDEF) doubleTheRateCorrectionIntervalSec;  /**< double the rate correction calculation interval - seconds part */
#endif /* STBM_SLAVE_RATE_CORRECTION_USED == STD_ON */
#if ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON))
  CONST(uint16, TYPEDEF) numberOfBlocks;  /**< number of Time Recording Blocks */
#endif /* ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_SYNC_TIME_RECORDING_USED == STD_ON)) */
#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
  CONST(uint16, TYPEDEF) numberOfBlocksTimeValidation;  /**< numberOfBlocksTimeValidation */
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */
#if (STBM_NVM_USED == STD_ON)
  CONST(uint16, TYPEDEF) NvMBlockId; /**< NVRAM Block Id */
#endif /* (STBM_NVM_USED == STD_ON) */
#if (STBM_MASTER_RATE_CORRECTION_USED == STD_ON)
  CONST(uint16, TYPEDEF) masterRateDeviationMax;  /**< maximum value of rate deviation */
  CONST(uint8, TYPEDEF) masterRcArrayIndex;  /**< index for time bases in the StbM_MasterRateCorrectionData[] array */
#endif /* STBM_MASTER_RATE_CORRECTION_USED == STD_ON */
#if (STBM_SLAVE_RATE_CORRECTION_USED == STD_ON)
  CONST(uint8, TYPEDEF) startIndexRcIntervals;  /**< start index in the array where start values are stored for
                                                     Rate Correction calculation */
  CONST(uint8, TYPEDEF) stopIndexRcIntervals;  /**< stop index in the array where start values are stored for
                                                    Rate Correction calculation */
  CONST(uint8, TYPEDEF) intervalArrayIndex;  /**< index for time bases in the StbM_SlaveRateCorrectionData[] array */
#endif /* STBM_SLAVE_RATE_CORRECTION_USED == STD_ON */
#if ((STBM_MASTER_RATE_CORRECTION_USED == STD_ON) || (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON))
  CONST(StbM_MasterConfigType, TYPEDEF) systemWideMaster;  /**< specifies if this time base is a System Wide Global Time Master */
#endif /* ((STBM_MASTER_RATE_CORRECTION_USED == STD_ON) || (STBM_GET_MASTERCONFIG_SUPPORTED == STD_ON)) */
#if (STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON)
  CONST(uint8, TYPEDEF) timeBaseSharesDataIdx;  /**< index for time bases in the StbM_SharedEthOffsetData.Offset[] array */
#endif /* STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON */

#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)
  CONST(uint8, TYPEDEF) numberOfNotificationCustomers;  /**< the number of notification customers per time base */
#endif /* (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON) */

} StbM_CommonTimeBaseCfgType;
#endif /* STBM_COMMON_FEATURES_ENABLED */


/** \brief Time Leap information
 **
 ** This type contains two counters, for future and past time leaps and the timeJump value.
 */
typedef struct
{
  VAR(boolean, TYPEDEF) timeJumpCalculated;  /* True if the timeJump was calculated at least once. */
#if (STBM_TIMELEAP_DETECTION_USED == STD_ON)
  VAR(uint16, TYPEDEF) future;  /**< clear timeleap counter for a future timeleap */
  VAR(uint16, TYPEDEF) past;  /**< clear timeleap counter for a past timeleap */
#endif /* (STBM_TIMELEAP_DETECTION_USED == STD_ON) */
  VAR(StbM_TimeDiffType, TYPEDEF) timejump;  /**< difference between the received time(TG) and the synclocal time(TL) */
} StbM_TimeLeapType;

#if (STBM_SLAVE_RATE_CORRECTION_USED == STD_ON)
/** \brief Time Stamp storage for Rate Correction
 **
 ** This type contains is used to store time stamps at run-time
 ** for all measurement intervals, for all time bases
 */
typedef struct
{
  VAR(StbM_TimeStampType, TYPEDEF) startGlobalTime;  /**< Current time of the Global Time Base Time Master(Sync Time Base) /
                                                     Current corrected time provided by the local instance of the associated
                                                     Time Base (Offset Time Base) */
  VAR(StbM_VirtualLocalTimeType, TYPEDEF) startVirtualLocalTime;  /**< Current time of the Virtual Local Time of the associated
                                                          Time Base (Sync Time Base) / Current Offset of the Offset Time Base
                                                          given as function parameter (Offset Time Base) */
} StbM_SlaveRateCorrectionStartValuesCfgType;

/** \brief Measurement interval for Rate Correction
 **
 ** This type is used to store the measurement interval at run-time
 ** for all time bases.
 */
typedef struct
{
  VAR(StbM_TimeTupleType, TYPEDEF) newGlobalTimeData;  /**< new Time Tuple provided by BusSetGlobalTime */
  VAR(StbM_VirtualLocalTimeType, TYPEDEF) syncVirtualLocalTime;  /**< previous virtual local time provided by
                                                                      the previous BusSetGlobalTime */
  VAR(uint32, TYPEDEF) rrc;  /**< rate correction with all calculated decimals */
#if (STBM_OFFSET_CORRECTION_USED == STD_ON)
  VAR(uint32, TYPEDEF) roc;  /**< rate adaptation with all calculated decimals */
#endif /* STBM_OFFSET_CORRECTION_USED == STD_ON */
  VAR(StbM_RateDeviationType, TYPEDEF) rateDev;  /**< rate deviation in ppm */
  VAR(uint8, TYPEDEF) intervalCounter;  /**< interval number */
  VAR(uint8, TYPEDEF) discardMeasurementCounter;  /**< intervals in a measurement that must be discarded */
  VAR(uint8, TYPEDEF) syncToGateway;  /**< keep track of SYNC_TO_GATEWAY flag */
  VAR(uint8, TYPEDEF) restart;  /**< value to check if measurements need to be restarted */
  VAR(uint8, TYPEDEF) updatedFlags;  /**< flags after the execution of StbM_BusSetGlobalTime */
  VAR(uint8, TYPEDEF) initialFlags;  /**< flags before the update of flags in StbM_BusSetGlobalTime */
  VAR(boolean, TYPEDEF) newValues;  /**< new time tuple available for rate correction */
#if (STBM_OFFSET_CORRECTION_USED == STD_ON)
  VAR(uint8, TYPEDEF) rocSign; /**< sign of the rate adaptation(could be negative) */
  VAR(boolean, TYPEDEF) beforeAdaptionInterval;  /**< keep track if we are before/after the Adaption interval */
#endif /* STBM_OFFSET_CORRECTION_USED == STD_ON */
} StbM_SlaveRateCorrectionDataType;
#endif /* STBM_SLAVE_RATE_CORRECTION_USED == STD_ON */

#if (STBM_MASTER_RATE_CORRECTION_USED == STD_ON)
/** \brief Measurement interval for Rate Correction
 **
 ** This type contains is used to store measurement interval at run-time
 ** for all time bases
 */
typedef struct
{
  VAR(uint32, TYPEDEF) rrc;  /**< rate correction with all calculated decimals */
  VAR(StbM_RateDeviationType, TYPEDEF) rateDev;  /**< rate deviation in ppm */
} StbM_MasterRateCorrectionDataType;
#endif /* STBM_MASTER_RATE_CORRECTION_USED == STD_ON */



#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)

/** \brief CustomerTimerExpireTime for each Notification Customer of a time base
 **
 ** This type is used to store timeBaseId, customerId and customerTimerExpireTime at run-time
 ** for each Notification Customer of a time base.
 */
typedef struct
{
  VAR(StbM_SynchronizedTimeBaseType, TYPEDEF) timeBaseId;  /**< Time Base Id */
  VAR(StbM_CustomerIdType, TYPEDEF) customerId;  /**< Customer Id */
  VAR(StbM_TimeStampType, TYPEDEF) customerTimerExpireTime; /**< calculated customerTimerExpireTime, based on the expireTime received through StbM_StartTimer and and the corresponding Time Base time */
  VAR(uint32, TYPEDEF) arrayCellIndex; /**< variable which states the index of the array */
  VAR(boolean, TYPEDEF) isGptTimerRunning; /**< variable which states if timer is running or not */
  VAR(boolean, TYPEDEF) pendingStartGptTimer; /**< variable which states if there is a pending call to Gpt_StartTimer */
  VAR(boolean, TYPEDEF) isTimerExpiredInGpt; /**< variable which states if timer expired or not */
  VAR(boolean, TYPEDEF) isArrayCellEmpty; /**< variable which states if the array cell is free or not */
  VAR(StbM_TimeDiffType, TYPEDEF) calculatedTimeDeviation; /**< time deviation which was calculated on StbM_TimerCallback */
} StbM_CustomerTimerExpireTimeDataType;




/** \brief CustomerTimerExpireTime for each Notification Customer of a time base
 **
 ** This type is used to store timeBaseId, customerId and customerTimerExpireTime at run-time
 ** for each Notification Customer of a time base.
 */
typedef struct StbM_CustomerTimerExpireTimeNodeDataType
{
  P2VAR(struct StbM_CustomerTimerExpireTimeNodeDataType, AUTOMATIC, STBM_APPL_DATA) previous; /**< pointer to the previous expiry point in the sorted list */
  VAR(StbM_CustomerTimerExpireTimeDataType, TYPEDEF) Data; /**< data related to customer timer expire time */
  P2VAR(struct StbM_CustomerTimerExpireTimeNodeDataType, AUTOMATIC, STBM_APPL_DATA) next; /**< pointer to the next expiry point in the sorted list */
}StbM_CustomerTimerExpireTimeNodeDataType;


#endif /* STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON */



#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
/** \brief Function pointer for Sync Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the measurement data
 ** table belonging to the Synchronized Time Base.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_SyncTimeRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_SyncRecordTableBlockType, AUTOMATIC, STBM_APPL_CONST) syncRecordTableBlock
);
#endif /* STBM_SYNC_TIME_RECORDING_USED == STD_ON */



#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)

#if (STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED == STD_ON)
/** \brief Function pointer for Sync Can Time Master Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_CanTimeMasterMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_CanTimeMasterRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_CanTimeMasterMeasurementType, AUTOMATIC, STBM_APPL_CONST) canTimeMasterTimeValidationRecordTableBlock
);
#endif /* (STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED == STD_ON) */


#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON)
/** \brief Function pointer for Sync Fr Time Master Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_FrTimeMasterMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_FrTimeMasterRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_FrTimeMasterMeasurementType, AUTOMATIC, STBM_APPL_CONST) frTimeMasterTimeValidationRecordTableBlock
);
#endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON) */


#if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)
/** \brief Function pointer for Sync Eth Time Master Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_EthTimeMasterMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_EthTimeMasterRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_EthTimeMasterMeasurementType, AUTOMATIC, STBM_APPL_CONST) ethTimeMasterTimeValidationRecordTableBlock
);

/** \brief Function pointer for Sync Eth Pdelay Responder Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_PdelayResponderMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_EthPdelayResponderRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_PdelayResponderMeasurementType, AUTOMATIC, STBM_APPL_CONST) ethPdelayResponderTimeValidationRecordTableBlock
);


/** \brief Function pointer for Sync Eth Time Slave Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_EthTimeSlaveMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_EthTimeSlaveRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_EthTimeSlaveMeasurementType, AUTOMATIC, STBM_APPL_CONST) ethTimeSlaveTimeValidationRecordTableBlock
);

/** \brief Function pointer for Sync Eth Pdelay Initiator Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_PdelayInitiatorMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_EthPdelayInitiatorRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_PdelayInitiatorMeasurementType, AUTOMATIC, STBM_APPL_CONST) ethPdelayInitiatorTimeValidationRecordTableBlock
);
#endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */




#if (STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED == STD_ON)
/** \brief Function pointer for Sync Can Time Slave Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_CanTimeSlaveMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_CanTimeSlaveRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_CanTimeSlaveMeasurementType, AUTOMATIC, STBM_APPL_CONST) canTimeSlaveTimeValidationRecordTableBlock
);
#endif /* (STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED == STD_ON) */


#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON)
/** \brief Function pointer for Sync Fr Time Slave Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the Time Validation measurement data
 ** of type StbM_FrTimeSlaveMeasurementType.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_FrTimeSlaveRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_FrTimeSlaveMeasurementType, AUTOMATIC, STBM_APPL_CONST) frTimeSlaveTimeValidationRecordTableBlock
);
#endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON) */


#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */




#if (STBM_STATUS_NOTIFICATIONS_USED == STD_ON)
/** \brief Function pointer for status notification callback
 **
 ** This type defines a function pointer, pointing to the API service
 ** for status notifications.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_StatusNotificationCbkFctPtrType)
(
  VAR(StbM_TimeBaseNotificationType, STBM_APPL_DATA) eventNotification
);
#endif /* STBM_STATUS_NOTIFICATIONS_USED == STD_ON */


#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)
/** \brief Function pointer for notification of customers callback
 **
 ** This type defines a function pointer, pointing to the API service
 ** for notification of customers.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_CustomerNotificationCbkFctPtrType)
(
  VAR(StbM_TimeDiffType, STBM_APPL_DATA) deviationTime
);
#endif /* STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON */


#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
/** \brief Function pointer for Offset Time Record Block Callback
 **
 ** Provides a recorded snapshot data block of the measurement data
 ** table belonging to the Offset Time Base.
 */
typedef P2FUNC (Std_ReturnType, STBM_APPL_CODE, StbM_OffsetTimeRecordBlockCbkFctPtrType)
(
  P2CONST(StbM_OffsetRecordTableBlockType, AUTOMATIC, STBM_APPL_CONST) offsetRecordTableBlock
);
#endif /* STBM_OFFSET_TIME_RECORDING_USED == STD_ON */

#if ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_STATUS_NOTIFICATIONS_USED == STD_ON) || (STBM_TIME_VALIDATION_SUPPORT == STD_ON))
/** \brief Callbacks config
 **
 ** This type contains all the callbacks of a certain
 ** timebase
 */
typedef struct
{
#if (STBM_STATUS_NOTIFICATIONS_USED == STD_ON)
  CONST(StbM_StatusNotificationCbkFctPtrType, TYPEDEF) statusNotificationCbkFctPtr;
#if (STBM_RTE_USED == STD_ON)
  CONST(StbM_StatusNotificationCbkFctPtrType, TYPEDEF) statusNotificationRteCbkFctPtr;
#endif /* (STBM_RTE_USED == STD_ON) */
#endif /* STBM_STATUS_NOTIFICATIONS_USED == STD_ON */

#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
  CONST(StbM_SyncTimeRecordBlockCbkFctPtrType, TYPEDEF) syncTimeRecordBlockCbkFctPtr;
#endif /* STBM_SYNC_TIME_RECORDING_USED == STD_ON */
#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
  CONST(StbM_OffsetTimeRecordBlockCbkFctPtrType, TYPEDEF) offsetTimeRecordBlockCbkFctPtr;
#endif /* STBM_OFFSET_TIME_RECORDING_USED == STD_ON */

#if ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON))
  CONST(StbM_SyncTimeRecordBlockCbkFctPtrType, TYPEDEF) syncTimeRecordBlockRteCbkFctPtr;
#endif /* (STBM_SYNC_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON) */
#if ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON))
  CONST(StbM_OffsetTimeRecordBlockCbkFctPtrType, TYPEDEF) offsetTimeRecordBlockRteCbkFctPtr;
#endif /* ((STBM_OFFSET_TIME_RECORDING_USED == STD_ON) && (STBM_RTE_USED == STD_ON)) */


#if (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
#if (STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED == STD_ON)
  CONST(StbM_CanTimeMasterRecordBlockCbkFctPtrType, TYPEDEF) canTSynMasterTimeValidationRecordBlockRteCbkFctPtr;
#endif /* (STBM_HAS_AT_LEAST_ONE_CANTSYN_MASTER_LINKED == STD_ON) */
#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON)
  CONST(StbM_FrTimeMasterRecordBlockCbkFctPtrType, TYPEDEF) frTSynMasterTimeValidationRecordBlockRteCbkFctPtr;
#endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_MASTER_LINKED == STD_ON) */
#if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)
  CONST(StbM_EthTimeMasterRecordBlockCbkFctPtrType, TYPEDEF) ethTSynMasterTimeValidationRecordBlockRteCbkFctPtr;
  CONST(StbM_EthPdelayResponderRecordBlockCbkFctPtrType, TYPEDEF) ethTSynPdelayResponderTimeValidationRecordBlockRteCbkFctPtr;
#endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */
#if (STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED == STD_ON)
  CONST(StbM_CanTimeSlaveRecordBlockCbkFctPtrType, TYPEDEF) canTSynSlaveTimeValidationRecordBlockRteCbkFctPtr;
#endif /* (STBM_HAS_AT_LEAST_ONE_CANTSYN_SLAVE_LINKED == STD_ON) */
#if (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON)
  CONST(StbM_FrTimeSlaveRecordBlockCbkFctPtrType, TYPEDEF) frTSynSlaveTimeValidationRecordBlockRteCbkFctPtr;
#endif /* (STBM_HAS_AT_LEAST_ONE_FRTSYN_SLAVE_LINKED == STD_ON) */
#if (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON)
  CONST(StbM_EthTimeSlaveRecordBlockCbkFctPtrType, TYPEDEF) ethTSynSlaveTimeValidationRecordBlockRteCbkFctPtr;
  CONST(StbM_EthPdelayInitiatorRecordBlockCbkFctPtrType, TYPEDEF) ethTSynPdelayInitiatorTimeValidationRecordBlockRteCbkFctPtr;
#endif /* (STBM_HAS_AT_LEAST_ONE_ETHTSYN_LINKED_TO_A_TIMEBASE_WHICH_HAS_TIMEVALIDATION_ENABLED == STD_ON) */
#endif /* (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */

} StbM_CallbacksFctPtrType;
#endif /* ((STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_STATUS_NOTIFICATIONS_USED == STD_ON) || (STBM_TIME_VALIDATION_SUPPORT == STD_ON)) */
#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)

/** \brief Notification of customer config
 **
 ** This type contains the precompile time configuration
 ** of a notification customer
 */
typedef struct
{
  CONST(StbM_SynchronizedTimeBaseType, TYPEDEF) timeBaseId;  /**< Time Base Id */
  CONST(StbM_CustomerIdType, TYPEDEF) customerId;  /**< Customer Id */
  CONST(StbM_CustomerNotificationCbkFctPtrType, TYPEDEF) customerNotificationCbkFctPtr; /**< Time Notification Callback or TimeNotification C-S Interface */
} StbM_CustomerNotificationCfgType;

/** \brief Notification of customer deviation data
 **
 ** This type contains identification and deviation data of a notification customer
 */
typedef struct
{
  VAR(StbM_SynchronizedTimeBaseType, TYPEDEF) timeBaseId;  /**< Time Base Id */
  VAR(StbM_CustomerIdType, TYPEDEF) customerId;  /**< Customer Id */
  VAR(StbM_TimeDiffType, TYPEDEF) calcDeviation; /**< Calculated deviation */
} StbM_SendCustomerNotificationType;

/** \brief Local Time function data
 **
 ** This type contains local Time function data
 */
typedef struct
{
  VAR(StbM_TimeStampType, TYPEDEF) localTime;  /**< Local Time */
  VAR(Std_ReturnType, TYPEDEF) retVal;  /**< Return of the Calculate Local Time function */
} StbM_LocalTimeCallType;

/** \brief Pair of data for Gpt_StartTimer() call
 **
 ** This type contains pair of data for Gpt_StartTimer() call
 */
typedef struct
{
  VAR(StbM_TimeStampType, TYPEDEF) expireTimeDiff;  /**< Diff to give to Gpt */
  VAR(uint32, TYPEDEF) arrayIndex;  /**< Index of the linked list element in the array */
} StbM_GptCallPairType;

#endif /* (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON) */

#if (STBM_TRIGGERED_CUSTOMERS_USED == STD_ON)
/** \brief Triggered customer config
 **
 ** This type contains the precompile time configuration
 ** of a triggered customer
 */
typedef struct
{
  CONST(uint32, TYPEDEF) invocationNumber;  /**< number of Mainfunction ticks between triggers */
  CONST(uint32, TYPEDEF) scheduleTblDuration;  /**< Duration of schedule table in ticks */
  CONST(ScheduleTableType, TYPEDEF) scheduleTblIndex;  /**< Index of the related Os schedule table */
  CONST(StbM_SynchronizedTimeBaseType, TYPEDEF) syncTimeBaseIndex;  /**< Index of the related synchronized time base */
} StbM_TriggerCustomerCfgType;

#endif /* (STBM_TRIGGERED_CUSTOMERS_USED == STD_ON) */

/*==[Constants with external linkage]=========================================*/

#define STBM_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <StbM_MemMap.h>

#if (STBM_OFFSET_TIMEBASES_USED == STD_ON)
/** \brief Offset time bases precompile time configuration structure */
extern CONST(StbM_OffsetTimeBaseCfgType, STBM_APPL_CONST)
  StbM_OffsetTimeBaseCfg[STBM_NUMBER_OF_OFFSET_TIMEBASES];
#endif /* #if (STBM_OFFSET_TIMEBASES_USED == STD_ON) */

#if (STBM_TRIGGERED_CUSTOMERS_USED == STD_ON)
/** \brief Triggered customer precompile time configuration structure */
extern CONST(StbM_TriggerCustomerCfgType, STBM_APPL_CONST)
  StbM_TriggerCustomerCfg[STBM_NUMBER_OF_TRIGGERED_CUSTOMERS];
#endif /* #if (STBM_TRIGGERED_CUSTOMERS_USED == STD_ON) */



#if (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON)
/** \brief Customer Notification precompile time configuration structure */
extern CONST(StbM_CustomerNotificationCfgType, STBM_APPL_CONST)
  StbM_CustomerNotificationCfg[STBM_NUMBER_OF_NOTIFICATION_CUSTOMERS];
#endif /* (STBM_NOTIFICATION_OF_CUSTOMERS_USED == STD_ON) */


/** \brief Synchronized time bases precompile time configuration structure */
extern CONST(StbM_SyncTimeBaseCfgType, STBM_APPL_CONST)
  StbM_SyncTimeBaseCfg[STBM_NUMBER_OF_SYNC_TIMEBASES + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES];

#if (STBM_GPT_USED == STD_ON)
/** \brief Time Bases that use Gpt as source for Virtual Local Time reference precompile time configuration structure */
extern CONST(StbM_GptTimeBaseCfgType, STBM_APPL_CONST)
  StbM_GptTimeBases[STBM_NUMBER_OF_GPT_TIMEBASES];
#endif /* #if (STBM_GPT_USED == STD_ON) */

/** \brief Function pointer list of all layers for setting the global time */
extern CONST(StbM_TimeSetterFctPtrType, STBM_APPL_CONST)
  StbM_TimeSetterFctPtrList[STBM_NUMBER_OF_ADJACENT_MODULES];

#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_STATUS_NOTIFICATIONS_USED == STD_ON) || (STBM_TIME_VALIDATION_SUPPORT == STD_ON)
/** \brief Function pointer list for time base status notification */
extern CONST(StbM_CallbacksFctPtrType, STBM_APPL_CONST)
  StbM_CallbacksPtrList[STBM_NUMBER_OF_SYNC_TIMEBASES + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES];
#endif /* (STBM_SYNC_TIME_RECORDING_USED == STD_ON) || (STBM_OFFSET_TIME_RECORDING_USED == STD_ON) || (STBM_STATUS_NOTIFICATIONS_USED == STD_ON) || (STBM_TIME_VALIDATION_SUPPORT == STD_ON) */

#if (STBM_COMMON_FEATURES_ENABLED)
/** \brief Function pointer list for time base status notification */
extern CONST(StbM_CommonTimeBaseCfgType, STBM_APPL_CONST)
  StbM_CommonTimeBaseCfg[STBM_NUMBER_OF_SYNC_TIMEBASES + STBM_NUMBER_OF_OFFSET_TIMEBASES + STBM_NUMBER_OF_PURE_TIMEBASES];
#endif /* STBM_COMMON_FEATURES_ENABLED */

#if (STBM_SYNC_TIME_RECORDING_USED == STD_ON)
extern CONST(StbM_SyncRecordTableHeadType, STBM_APPL_CONST)
  StbM_SyncRecordTableHeads[STBM_NUMBER_OF_SYNC_TIMEBASES];
#endif /* STBM_SYNC_TIME_RECORDING_USED == STD_ON */

#if (STBM_OFFSET_TIME_RECORDING_USED == STD_ON)
extern CONST(StbM_OffsetRecordTableHeadType, STBM_APPL_CONST)
  StbM_OffsetRecordTableHeads[STBM_NUMBER_OF_OFFSET_TIMEBASES];
#endif /* STBM_OFFSET_TIME_RECORDING_USED == STD_ON */

#define STBM_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <StbM_MemMap.h>

#if (STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON)

#define STBM_START_SEC_VAR_SHARED_TIME_DATA
#include <StbM_MemMap.h>

  /** \brief Variable that holds the Memory Shared Data of the Time Bases */
extern VAR(StbM_SharedEthOffsetType, STBM_VAR) StbM_SharedEthOffsetData;

#define STBM_STOP_SEC_VAR_SHARED_TIME_DATA
#include <StbM_MemMap.h>

#endif /* STBM_TIME_BASES_THAT_SHARE_DATA_USED == STD_ON */

/*==[Variables with external linkage]=========================================*/

/*==[Definition of functions with external linkage]===========================*/

#define STBM_START_SEC_CODE
#include <StbM_MemMap.h>

/** \brief Updates synchronized time base derived from an Os counter\EthTSyn\Gpt Channel
 **
 ** This function updates the actual time for a synchronized time base which is based
 ** on an Os counter, the EthTSyn module or a Gpt Channel.
 **
 ** \param[in] syncTimeBaseIndex - Id (=index) of the synchronized, Os counter based,
 **                                time base
 ** \param[out] timeStampPtr - Pointer to current timestamp
 ** \param[out] currentTimeTupleGlobalPtr - Current stored Global Time value.
 ** \param[out] currentTimeTupleVLTPtr - Current stored Virtual Local Time value.
 ** \param[out] localTimePtr - current virtual local time stamp
 **
 ** \return if the time base was successfully updated
 ** \retval E_OK update successful
 ** \retval E_NOT_OK update failed
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentTimeInternal
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr,
  P2VAR(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) currentTimeTupleGlobalPtr,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) currentTimeTupleVLTPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);

/** \brief Set Os counter\EthTSyn based time base
 **
 ** This function sets the actual time for a synchronized time base which is based
 ** on an Os counter or the EthTSyn module.
 **
 ** \param[in] syncTimeBaseIndex - Id (=index) of the synchronized, Os counter based,
 **                                time base
 ** \param[out] timeStampPtr - Pointer to current timestamp
 **
 ** \return if the time base was successfully set
 ** \retval E_OK successful
 ** \retval E_NOT_OK failed
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_SetGlobalTimeOffset
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2CONST(StbM_TimeStampType, AUTOMATIC, STBM_APPL_DATA) timeStampPtr,
  P2CONST(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);

#if (STBM_OS_COUNTER_USED == STD_ON)
/** \brief Enables virtual local time updates from Os counter
 **
 ** Obtain the virtual local time from the Os counter
 **
 ** \param[in] timeBaseId - Id of the synchronized time base
 ** \param[out] localTimePtr - Points to the memory location where the virtual
 **                            local time is stored
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentVirtualLocalTime_BasedOn_OsCounter
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR( StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);
#endif /* STBM_OS_COUNTER_USED == STD_ON */


#if (STBM_OSGETTIMESTAMP_USED == STD_ON)
/** \brief Enables virtual local time updates from Os, through OSGetTimeStamp API
 **
 ** Obtain the virtual local time from the Os, through OSGetTimeStamp API
 **
 ** \param[in] timeBaseId - Id of the synchronized time base
 ** \param[out] localTimePtr - Points to the memory location where the virtual
 **                            local time is stored
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentVirtualLocalTime_Using_OSGetTimeStamp
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR( StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);
#endif /* STBM_OSGETTIMESTAMP_USED == STD_ON */



#if (STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED == STD_ON)
/** \brief Enables virtual local time updates from EthTSyn
 **
 ** Obtain the virtual local time from the EthTSyn module
 **
 ** \param[in] timeBaseId - Id of the synchronized time base
 ** \param[out] localTimePtr - Points to the memory location where the virtual
 **                            local time is stored
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentVirtualLocalTimeEthTSyn
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR(StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);
#endif /* STBM_VIRTUAL_LOCALTIME_ETHTSYN_USED == STD_ON */

#if (STBM_GPT_USED == STD_ON)
/** \brief Enables virtual local time updates from Gpt
 **
 ** Obtain the virtual local time from Gpt
 **
 ** \param[in] timeBaseId - Id of the synchronized time base
 ** \param[out] localTimePtr - Points to the memory location where the virtual
 **                            local time is stored
 **
 ** \return the success/failure of the function call
 ** \retval E_OK In case of successful call of the function.
 ** \retval E_NOT_OK In case of unsuccessful call of the function.
 */
extern FUNC(Std_ReturnType, STBM_CODE) StbM_GetCurrentVirtualLocalTimeGpt
(
  StbM_SynchronizedTimeBaseType timeBaseId,
  P2VAR( StbM_VirtualLocalTimeType, AUTOMATIC, STBM_APPL_DATA) localTimePtr
);
#endif /* STBM_GPT_USED == STD_ON */

[!IF "$StbMTriggeredCustomersNumber > 0"!]
/** \brief Convert nanoseconds to Os counter ticks
 **
 ** This generated function converts the given nanoseconds value to a tick
 ** value dependent on the used underlying Os counter of the given
 ** triggered customer.
 **
 ** \param[in] triggeredCustomerIndex - (sorted) zero-based index of the triggered customer
 ** \param[in] nanoseconds - nanoseconds value for conversion into ticks
 **
 ** \return converted tick value
 */
extern FUNC(uint32, STBM_CODE) StbM_ConvNsToTicks
(
  uint8 triggeredCustomerIndex,
  uint32 nanoseconds
);
[!ENDIF!]



#if (STBM_GPT_USED == STD_ON)
/** \brief Convert nanoseconds to Os counter ticks
 **
 ** This generated function converts the given nanoseconds value to a tick
 ** value dependent on the used underlying Os counter of the given
 ** triggered customer.
 **
 ** \param[in] timeBaseId - the ID of the Time Base
 ** \param[in] ticks - ticks which will be converted into nanoseconds
 ** \param[in] frequencyDivByPrescalerFactor - frequency divided by prescaler
 **
 ** \return converted nanoseconds value
 */
extern FUNC(uint32, STBM_CODE) StbM_ConvGptTicksToNs
(
  uint32 timeBaseId,
  uint32 ticks
);
#endif /* (STBM_GPT_USED == STD_ON) */


#define STBM_STOP_SEC_CODE
#include <StbM_MemMap.h>

/** @} doxygen end group definition */
/*==================[end of file]===========================================*/


#endif /* ifndef STBM_CFG_INT_H */
