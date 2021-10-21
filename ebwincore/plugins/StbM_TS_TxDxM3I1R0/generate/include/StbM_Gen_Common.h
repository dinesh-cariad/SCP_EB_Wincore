[!AUTOSPACING!]
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
#ifndef STBM_GEN_COMMON_H
#define STBM_GEN_COMMON_H

/*==================[inclusions]============================================*/

[!INCLUDE "StbM_Vars.m"!]

/*==================[macros]================================================*/

#if (defined STBM_INSTANCE_ID)
#error STBM_INSTANCE_ID is already defined.
#endif
/** \brief StbM instance id */
#define STBM_INSTANCE_ID                        0U


/* API service ID of StbM_MainFunction */
#if (defined STBM_API_MAINFUNCTION)
#error STBM_API_MAINFUNCTION is already defined.
#endif
/** \brief Service ID of StbM_MainFunction() */
/* !LINKSTO StbM.ASR44.SWS_StbM_00057,1 */
#define STBM_API_MAINFUNCTION                   0x04U


#if (defined STBM_RTE_USED)
#error STBM_RTE_USED is already defined.
#endif
/** \brief Enables/disables access to Rte. */
#define STBM_RTE_USED                             STD_[!IF "StbMGeneral/StbMRteUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/* !LINKSTO StbM.ASR44.SWS_StbM_00307,1 */
#if (defined STBM_TIME_RECORDING_USED)
#error STBM_TIME_RECORDING_USED is already defined.
#endif
/** \brief Enables/disables access to Offset Time Record for time bases. */
#define STBM_TIME_RECORDING_USED                  STD_[!IF "StbMGeneral/StbMTimeRecordingSupport = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_TIME_VALIDATION_SUPPORT)
#error STBM_TIME_VALIDATION_SUPPORT is already defined.
#endif
/** \brief Enables/disabled Time Validation support */
#define STBM_TIME_VALIDATION_SUPPORT       STD_[!IF "$StbMTimeValidationUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/* !LINKSTO StbM.EB.ECUC_StbM_00063,1 */
#if (defined STBM_NOTIFICATION_OF_CUSTOMERS_USED)
#error STBM_NOTIFICATION_OF_CUSTOMERS_USED is already defined.
#endif
/** \brief Enables/disables notification of customers, when a Time Base reaches the time value set by StbM_StartTimer */
#define STBM_NOTIFICATION_OF_CUSTOMERS_USED                  STD_[!IF "node:exists(StbMGeneral/StbMTimerStartThreshold) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


/* !LINKSTO StbM.EB.ECUC_StbM_00063,1 */
#if (defined STBM_GETCURRENTTIMEEXTENDED_USED)
#error STBM_GETCURRENTTIMEEXTENDED_USED is already defined.
#endif
/** \brief Enables/disables the usage of the StbM_GetCurrentTimeExtended API */
#define STBM_GETCURRENTTIMEEXTENDED_USED                  STD_[!IF "node:value(StbMGeneral/StbMGetCurrentTimeExtendedAvailable) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_SYNC_TIME_RECORDING_USED)
#error STBM_SYNC_TIME_RECORDING_USED is already defined.
#endif
/** \brief Enables/disables access to Sync Time Record for time bases. */
#define STBM_SYNC_TIME_RECORDING_USED                STD_[!IF "$StbMTimeBaseSyncTimeRecord = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_OFFSET_TIME_RECORDING_USED)
#error STBM_OFFSET_TIME_RECORDING_USED is already defined.
#endif
/** \brief Enables/disables access to Offset Time Record for time bases. */
#define STBM_OFFSET_TIME_RECORDING_USED              STD_[!IF "$StbMTimeBaseOffsetTimeRecord = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_NVM_USED)
#error STBM_NVM_USED is already defined.
#endif
/** \brief Enables/disables storage and loading of the TimeStamp in NvM module. */
#define STBM_NVM_USED              STD_[!IF "$StbMNvMUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED)
#error STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED is already defined.
#endif
/** \brief Enables/disables the declaration of the StbM_GetTimeLeap API in StbM module. */
#define STBM_SYNC_SLAVE_TIMEBASES_CONFIGURED              STD_[!IF "$StbMTimeBaseSyncSlaveExists = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_NUMBER_OF_SYNC_TIMEBASES)
#error STBM_NUMBER_OF_SYNC_TIMEBASES is already defined.
#endif
/** \brief Number of configured synchronized time bases. */
#define STBM_NUMBER_OF_SYNC_TIMEBASES             [!"$StbMSyncTimebaseNumber"!]U

#if (defined STBM_NUMBER_OF_PURE_TIMEBASES)
#error STBM_NUMBER_OF_PURE_TIMEBASES is already defined.
#endif
/** \brief Number of configured pure time bases. */
#define STBM_NUMBER_OF_PURE_TIMEBASES             [!"$StbMPureTimebaseNumber"!]U

#if (defined STBM_NUMBER_OF_OFFSET_TIMEBASES)
#error STBM_NUMBER_OF_OFFSET_TIMEBASES is already defined.
#endif
/** \brief Number of configured offset time bases. */
#define STBM_NUMBER_OF_OFFSET_TIMEBASES           [!"$StbMOffsetTimebaseNumber"!]U


#if (defined STBM_MASTER_RATE_CORRECTION_USED)
#error STBM_MASTER_RATE_CORRECTION_USED is already defined.
#endif
/** \brief Enables/disables Master Rate Correction calculation. */
#define STBM_MASTER_RATE_CORRECTION_USED          STD_[!IF "$StbMMasterRateCorrectionUsage = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

#if (defined STBM_GET_MASTERCONFIG_SUPPORTED)
#error STBM_GET_MASTERCONFIG_SUPPORTED is already defined.
#endif
/** \brief Enables/disables access to StbM_GetMasterConfig API */
#define STBM_GET_MASTERCONFIG_SUPPORTED                STD_[!IF "$StbMGetMasterConfigSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_SETOFFSET_SUPPORTED)
#error STBM_SETOFFSET_SUPPORTED is already defined.
#endif
/** \brief Enables/disables access to StbM_SetOffset APIs */
#define STBM_SETOFFSET_SUPPORTED                STD_[!IF "$atLeastOneOffsetMasterOrGatewayTimeBase = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_TRIGGERTIMETRANSMISSION_SUPPORTED)
#error STBM_TRIGGERTIMETRANSMISSION_SUPPORTED is already defined.
#endif
/** \brief Enables/disables access to StbM_TriggerTimeTransmission API */
#define STBM_TRIGGERTIMETRANSMISSION_SUPPORTED                STD_[!IF "($atLeastOneSyncOrOffsetMasterTimeBase = 'true')"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED)
#error STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED is already defined.
#endif
/** \brief Enables/disables access to StbM_SetGlobalTime, StbM_UpdateGlobalTime and StbM_SetUserData APIs */
#define STBM_MASTER_OR_GATEWAY_TIME_BASE_CONFIGURED                STD_[!IF "($atLeastOneMasterOrGatewayTimeBase = 'true')"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined STBM_MULTICORE_SUPPORTED)
#error STBM_MULTICORE_SUPPORTED is already defined.
#endif
/** \brief Enables/disables multicore support */
#define STBM_MULTICORE_SUPPORTED                  STD_[!IF "node:value(StbMGeneral/StbMMulticoreSupport) = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#endif /* ifndef STBM_GEN_COMMON_H */
/*==================[end of file]===========================================*/
