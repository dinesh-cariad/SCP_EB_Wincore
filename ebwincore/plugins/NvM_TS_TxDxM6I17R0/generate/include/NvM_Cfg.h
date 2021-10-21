/**
 * \file
 *
 * \brief AUTOSAR NvM
 *
 * This file contains the implementation of the AUTOSAR
 * module NvM.
 *
 * \version 6.17.22
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!INCLUDE "NvM_Include.m"!][!//
/* !LINKSTO NVM030,1 */
[!INCLUDE "NvM_Checks.m"!][!//
#ifndef NVM_CFG_H
#define NVM_CFG_H
/* !LINKSTO NVM028,1 */
/*==================[inclusions]============================================*/

/* !LINKSTO NVM689,1 */
#include <NvM_Types.h>        /* contains NvM specific types. NVM689 */
#include <Std_Types.h>

[!/* Each configured NvM block should have corresponding number of blocks configured in Fee or Ea */!]
[!/* Their BlockNumber has to match with NvMNvBlockBaseNumber + DataSetNumber */!]
[!LOOP "NvMBlockDescriptor/*[(NvMNvramBlockIdentifier > 1) and (NvMNvBlockNum > 1)]"!][!/*
*/!][!IF "NvMNvramBlockIdentifier = '1'"!][!/*
*/!][!VAR "Block1Size"="NvMNvBlockLength"!][!/*
*/!][!ENDIF!][!/*
*/!][!VAR "blockDesc"="NvMNvBlockNum"!][!/*
*/!][!IF "(node:exists(NvMTargetBlockReference/NvMNameOfFeeBlock))"!][!/*
*/!][!VAR "blockNumber"="node:ref(NvMTargetBlockReference/NvMNameOfFeeBlock)/FeeBlockNumber"!][!/*
*/!][!ELSEIF "(node:exists(NvMTargetBlockReference/NvMNameOfEaBlock))"!][!/*
*/!][!VAR "blockNumber"="node:ref(NvMTargetBlockReference/NvMNameOfEaBlock)/EaBlockNumber"!][!/*
*/!][!ENDIF!][!/*
*/!][!FOR "I"="0" TO "$blockDesc - 1"!][!/*
*/!][!IF "(node:exists(NvMTargetBlockReference/NvMNameOfFeeBlock)) and
          (count(node:ref(NvMTargetBlockReference/NvMNameOfFeeBlock)/../../FeeBlockConfiguration/*[FeeBlockNumber = ($blockNumber + $I)]) != 1)"!][!/*
*/!][!ERROR!]There should exist exactly one block in Fee with Block number = [!"num:i($blockNumber + $I)"!] for [!"node:name(.)"!][!ENDERROR!][!/*
*/!][!ELSEIF "(node:exists(NvMTargetBlockReference/NvMNameOfEaBlock)) and
              (count(node:ref(NvMTargetBlockReference/NvMNameOfEaBlock)/../../EaBlockConfiguration/*[EaBlockNumber = ($blockNumber + $I)]) != 1)"!][!/*
*/!][!ERROR!]There should exist exactly one block in Ea with Block number = [!"num:i($blockNumber + $I)"!] for [!"node:name(.)"!][!ENDERROR!][!/*
*/!][!ENDIF!][!/*
*/!][!ENDFOR!][!/*
*/!][!ENDLOOP!]
/*==================[macros]================================================*/
[!SELECT "as:modconf('Fee')[1]/FeeGeneral"!]
#if (defined  NVM_VARIABLE_BLOCK_LENGTH_EN)
#error NVM_VARIABLE_BLOCK_LENGTH_EN is already defined
#endif /* if (defined NVM_VARIABLE_BLOCK_LENGTH_EN) */
/** \brief Variable Block Length is Enabled.  */
#define NVM_VARIABLE_BLOCK_LENGTH_EN   [!//
[!IF "FeeDynamicBlockLength = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDSELECT!]

#if (defined NVM_DEV_ERROR_DETECT)
#error NVM_DEV_ERROR_DETECT is already defined
#endif
/** \brief Macro switch for DET usage */
#define NVM_DEV_ERROR_DETECT             [!//
[!IF "NvMCommon/NvMDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_INCLUDE_RTE)
#error NVM_INCLUDE_RTE is already defined
#endif
/** \brief Defines whether a Rte module configuration is available.
 **        If it is available then it is automatically used by the NVRAM Manager.
 **        STD_ON:  Rte configuration is available.
 **        STD_OFF: Rte configuration is not available.
 **/
#define NVM_INCLUDE_RTE                  [!//
[!IF "NvMCommon/NvMRteUsage = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_SET_RAM_BLOCK_STATUS_API)
#error NVM_SET_RAM_BLOCK_STATUS_API is already defined
#endif
/** \brief Disables or enables the API NvM_SetRamBlockStatus()
 **/
#define NVM_SET_RAM_BLOCK_STATUS_API     [!//
[!IF "(NvMCommon/NvMSetRamBlockStatusApi = 'true') and (count(NvMBlockDescriptor/*[(NvMBlockUseSetRamBlockStatus = 'true')]) != 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro to check if Block Checks is configured for any block
 **/
#define NVM_BLOCK_CHECK_ENABLED     [!//
[!IF "(count(NvMBlockDescriptor/*[(NvMEnBlockCheck = 'true')]) != 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Number of blocks configured with Block Check.
 **/
#define NVM_BC_NR_OF_BLOCKS     [!"num:i(count(NvMBlockDescriptor/*[(NvMEnBlockCheck = 'true')]))"!]U

 /** \brief Number of blocks configured with Block Check of type REDUNDANT.
**/
#define NVM_BC_NR_OF_BLOCKS_REDUNDANT     [!"num:i(count(NvMBlockDescriptor/*[(NvMEnBlockCheck = 'true') and (NvMBlockManagementType  = 'NVM_BLOCK_REDUNDANT')]))"!]U

/** \brief Macro to check if Block Id-s are consecutive. Otherwise a redirecting array is needed
 **/
#define NVM_USE_REDIR_BLOCK_ARRAY     [!//
[!IF "(count(NvMBlockDescriptor/*) != num:max(NvMBlockDescriptor/*/NvMNvramBlockIdentifier)) and (num:max(NvMBlockDescriptor/*/NvMNvramBlockIdentifier) div (count(NvMBlockDescriptor/*) + 1) <= 10)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro to check if Block Id-s are consecutive. Otherwise a binary search is needed
 **/
#define NVM_BLOCK_INDEX_SEARCH     [!//
[!IF "(num:max(NvMBlockDescriptor/*/NvMNvramBlockIdentifier) div (count(NvMBlockDescriptor/*) + 1) > 10)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Macro to check if Block Checks Set API is configured for any block
 **/
#define NVM_BLOCK_CHECK_SET_API_ENABLED     [!//
[!IF "(count(NvMBlockDescriptor/*[(NvMBcEnSetAPI = 'true')]) != 0)"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_CANCEL_INTERNAL_JOB)
#error NVM_CANCEL_INTERNAL_JOB is already defined
#endif
/** \brief Disables or enables the option to cancel an ongoing internal job.
 **/
#define NVM_CANCEL_INTERNAL_JOB             [!//
[!IF "NvMCommon/NvMCancelInternalOperations = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_VERSION_INFO_API)
#error NVM_VERSION_INFO_API is already defined
#endif
/** \brief Disables or enables the API NvM_VersionInfoApi()
 **/
#define NVM_VERSION_INFO_API             [!//
[!IF "NvMCommon/NvMVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_ADVANCED_RECOVERY)
#error NVM_ADVANCED_RECOVERY is already defined
#endif
/** \brief Dis- or enable the recovery of blocks()
 **/
#define NVM_ADVANCED_RECOVERY            [!//
[!IF "num:i(count(NvMBlockDescriptor/*[NvMAdvancedRecovery = 'true'])) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED)
#error NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting Integrity Failure
 **/
#define NVM_PROD_ERR_HANDLING_INTEGRITY_FAILED     [!//
[!IF "ReportToDem/NvMIntegrityFailedReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMIntegrityFailedReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMIntegrityFailedReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined NVM_PROD_ERR_HANDLING_REQ_FAILED)
#error NVM_PROD_ERR_HANDLING_REQ_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting Request Failure
 **/
#define NVM_PROD_ERR_HANDLING_REQ_FAILED           [!//
[!IF "ReportToDem/NvMRequestFailedReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMRequestFailedReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMRequestFailedReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY)
#error NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY already defined
#endif
/** \brief Switch for DEM to DET reporting Loss of Redundancy
 **/
#define NVM_PROD_ERR_HANDLING_LOSS_OF_REDUNDANCY   [!//
[!IF "ReportToDem/NvMLossOfRedundancyReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMLossOfRedundancyReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMLossOfRedundancyReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW)
#error NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW already defined
#endif
/** \brief Switch for DEM to DET reporting Queue Overflow
 **/
#define NVM_PROD_ERR_HANDLING_QUEUE_OVERFLOW       [!//
[!IF "ReportToDem/NvMQueueOverflowReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMQueueOverflowReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMQueueOverflowReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined NVM_PROD_ERR_HANDLING_VERIFY_FAILED)
#error NVM_PROD_ERR_HANDLING_VERIFY_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting Verify Failure
 **/
#define NVM_PROD_ERR_HANDLING_VERIFY_FAILED        [!//
[!IF "ReportToDem/NvMVerifyFailedReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMVerifyFailedReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMVerifyFailedReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined NVM_PROD_ERR_HANDLING_WRITE_PROTECTED)
#error NVM_PROD_ERR_HANDLING_WRITE_PROTECTED already defined
#endif
/** \brief Switch for DEM to DET reporting Write Protection
 **/
#define NVM_PROD_ERR_HANDLING_WRITE_PROTECTED      [!//
[!IF "ReportToDem/NvMWriteProtectedReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMWriteProtectedReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMWriteProtectedReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

#if (defined NVM_PROD_ERR_HANDLING_BLOCK_CHECK)
#error NVM_PROD_ERR_HANDLING_BLOCK_CHECK already defined
#endif
/** \brief Switch for DET reporting Block Check
 **/
#define NVM_PROD_ERR_HANDLING_BLOCK_CHECK      [!//
[!IF "ReportToDem/NvMBlockCheckReportProdError = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMBlockCheckReportProdError = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMBlockCheckReportProdError = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//


#if (defined NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID)
#error NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID already defined
#endif
/** \brief Switch for DEM to DET reporting Wrong Block ID
 **/
#define NVM_PROD_ERR_HANDLING_WRONG_BLOCK_ID       [!//
[!IF "ReportToDem/NvMWrongBlockIdReportToDem = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "ReportToDem/NvMWrongBlockIdReportToDem = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSEIF "ReportToDem/NvMWrongBlockIdReportToDem = 'UserCallout'"!][!//
TS_PROD_ERR_REP_TO_USER
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMIntegrityFailedReportToDem = 'DET'"!][!//

#if (defined NVM_E_DEMTODET_INTEGRITY_FAILED)
#error NVM_E_DEMTODET_INTEGRITY_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting Integrity Failure
 **/
#define NVM_E_DEMTODET_INTEGRITY_FAILED            [!"num:i(ReportToDem/NvMIntegrityFailedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMIntegrityFailedReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_INTEGRITY_FAILED)
#error NVM_E_USER_CALLOUT_INTEGRITY_FAILED already defined
#endif
/** \brief Switch for User Callout reporting Integrity Failure
 **/
#define NVM_E_USER_CALLOUT_INTEGRITY_FAILED            [!"num:i(ReportToDem/NvMIntegrityFailedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMRequestFailedReportToDem = 'DET'"!][!//

#if (defined NVM_E_DEMTODET_REQ_FAILED)
#error NVM_E_DEMTODET_REQ_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting Request Failure
 **/
#define NVM_E_DEMTODET_REQ_FAILED                  [!"num:i(ReportToDem/NvMRequestFailedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMRequestFailedReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_REQ_FAILED)
#error NVM_E_USER_CALLOUT_REQ_FAILED already defined
#endif
/** \brief Switch for User Callout reporting Request Failure
 **/
#define NVM_E_USER_CALLOUT_REQ_FAILED                  [!"num:i(ReportToDem/NvMRequestFailedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMLossOfRedundancyReportToDem = 'DET'"!][!//

#if (defined NVM_E_DEMTODET_LOSS_OF_REDUNDANCY)
#error NVM_E_DEMTODET_LOSS_OF_REDUNDANCY already defined
#endif
/** \brief Switch for DEM to DET reporting Loss of Redundancy
 **/
#define NVM_E_DEMTODET_LOSS_OF_REDUNDANCY          [!"num:i(ReportToDem/NvMLossOfRedundancyReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMLossOfRedundancyReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_LOSS_OF_REDUNDANCY)
#error NVM_E_USER_CALLOUT_LOSS_OF_REDUNDANCY already defined
#endif
/** \brief Switch for User Callout reporting Loss of Redundancy
 **/
#define NVM_E_USER_CALLOUT_LOSS_OF_REDUNDANCY          [!"num:i(ReportToDem/NvMLossOfRedundancyReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMQueueOverflowReportToDem = 'DET'"!][!//

#if (defined NVM_E_DEMTODET_QUEUE_OVERFLOW)
#error NVM_E_DEMTODET_QUEUE_OVERFLOW already defined
#endif
/** \brief Switch for DEM to DET reporting Queue Overflow
 **/
#define NVM_E_DEMTODET_QUEUE_OVERFLOW              [!"num:i(ReportToDem/NvMQueueOverflowReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMQueueOverflowReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_QUEUE_OVERFLOW)
#error NVM_E_USERCALLOUT_QUEUE_OVERFLOW already defined
#endif
/** \brief Switch for User Callout reporting Queue Overflow
 **/
#define NVM_E_USER_CALLOUT_QUEUE_OVERFLOW              [!"num:i(ReportToDem/NvMQueueOverflowReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMVerifyFailedReportToDem = 'DET'"!][!//

#if (defined NVM_E_DEMTODET_VERIFY_FAILED)
#error NVM_E_DEMTODET_VERIFY_FAILED already defined
#endif
/** \brief Switch for DEM to DET reporting Verify Failure
 **/
#define NVM_E_DEMTODET_VERIFY_FAILED               [!"num:i(ReportToDem/NvMVerifyFailedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMVerifyFailedReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_VERIFY_FAILED)
#error NVM_E_USER_CALLOUT_VERIFY_FAILED already defined
#endif
/** \brief Switch for User Callout reporting Verify Failure
 **/
#define NVM_E_USER_CALLOUT_VERIFY_FAILED               [!"num:i(ReportToDem/NvMVerifyFailedReportToDemDetErrorId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/NvMBlockCheckReportProdError = 'DET'"!][!//
#if (defined NVM_E_BLOCK_CHECK)
#error NVM_E_BLOCK_CHECK already defined
#endif
/** \brief Switch for DEM to DET reporting Block Check
 **/
#define NVM_E_BLOCK_CHECK             [!"num:i(ReportToDem/NvMBlockCheckReportProdErrorId)"!]U
[!ENDIF!][!//

[!IF "ReportToDem/NvMWriteProtectedReportToDem = 'DET'"!][!//
#if (defined NVM_E_DEMTODET_WRITE_PROTECTED)
#error NVM_E_DEMTODET_WRITE_PROTECTED already defined
#endif
/** \brief Switch for DEM to DET reporting Write Protection
 **/
#define NVM_E_DEMTODET_WRITE_PROTECTED             [!"num:i(ReportToDem/NvMWriteProtectedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMBlockCheckReportProdError = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_BLOCK_CHECK)
#error NVM_E_USER_CALLOUT_BLOCK_CHECK already defined
#endif
/** \brief Switch for User Callout reporting Write Protection
 **/
#define NVM_E_USER_CALLOUT_BLOCK_CHECK             [!"num:i(ReportToDem/NvMBlockCheckReportProdErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMWriteProtectedReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_WRITE_PROTECTED)
#error NVM_E_USER_CALLOUT_WRITE_PROTECTED already defined
#endif
/** \brief Switch for User Callout reporting Write Protection
 **/
#define NVM_E_USER_CALLOUT_WRITE_PROTECTED             [!"num:i(ReportToDem/NvMWriteProtectedReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMWrongBlockIdReportToDem = 'DET'"!][!//

#if (defined NVM_E_DEMTODET_WRONG_BLOCK_ID)
#error NVM_E_DEMTODET_WRONG_BLOCK_ID already defined
#endif
/** \brief Switch for DEM to DET reporting Wrong Block ID
 **/
#define NVM_E_DEMTODET_WRONG_BLOCK_ID              [!"num:i(ReportToDem/NvMWrongBlockIdReportToDemDetErrorId)"!]U
[!ENDIF!][!/*

*/!][!IF "ReportToDem/NvMWrongBlockIdReportToDem = 'UserCallout'"!][!//

#if (defined NVM_E_USER_CALLOUT_WRONG_BLOCK_ID)
#error NVM_E_USER_CALLOUT_WRONG_BLOCK_ID already defined
#endif
/** \brief Switch for User Callout reporting Wrong Block ID
 **/
#define NVM_E_USER_CALLOUT_WRONG_BLOCK_ID              [!"num:i(ReportToDem/NvMWrongBlockIdReportToDemDetErrorId)"!]U
[!ENDIF!]

#if (defined NVM_INVALID_DEM_EVENTID)
#error NVM_INVALID_DEM_EVENTID is already defined
#endif
/** \brief special value: no Dem event id configured
 **        (0 is not a valid value for Dem_EventIdType)
 **/
#define NVM_INVALID_DEM_EVENTID        0U

#if (defined NVM_INTEGRITY_FAILED_DEM_EVENT_ID)
#error NVM_INTEGRITY_FAILED_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for integrity failure Dem event Id
 **/
#define NVM_INTEGRITY_FAILED_DEM_EVENT_ID [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_INTEGRITY_FAILED)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_INTEGRITY_FAILED)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#if (defined NVM_REQ_FAILED_DEM_EVENT_ID)
#error NVM_REQ_FAILED_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for request failure Dem event Id
 **/
#define NVM_REQ_FAILED_DEM_EVENT_ID    [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_REQ_FAILED)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_REQ_FAILED)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#if (defined NVM_REDUNDANCY_LOSS_DEM_EVENT_ID)
#error NVM_REDUNDANCY_LOSS_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for redundancy loss Dem event Id
 **/
#define NVM_REDUNDANCY_LOSS_DEM_EVENT_ID [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_LOSS_OF_REDUNDANCY)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_LOSS_OF_REDUNDANCY)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#if (defined NVM_QUEUE_OVERFLOW_DEM_EVENT_ID)
#error NVM_QUEUE_OVERFLOW_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for queue overflow Dem event Id
 **/
#define NVM_QUEUE_OVERFLOW_DEM_EVENT_ID [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_QUEUE_OVERFLOW)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_QUEUE_OVERFLOW)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#if (defined NVM_VERIFY_FAILED_DEM_EVENT_ID)
#error NVM_VERIFY_FAILED_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for verify failed Dem event Id
 **/
#define NVM_VERIFY_FAILED_DEM_EVENT_ID [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_VERIFY_FAILED)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_VERIFY_FAILED)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#if (defined NVM_WRITE_PROT_DEM_EVENT_ID)
#error NVM_WRITE_PROT_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for write protection failure Dem event Id
 **/
#define NVM_WRITE_PROT_DEM_EVENT_ID    [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_WRITE_PROTECTED)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_WRITE_PROTECTED)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#define NVM_BLOCK_CHECK_DEM_EVENT_ID    [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_BLOCK_CHECK)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_BLOCK_CHECK)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//

#if (defined NVM_WRONG_BLOCK_DEM_EVENT_ID)
#error NVM_WRONG_BLOCK_DEM_EVENT_ID is already defined
#endif
/** \brief Switch for wrong block Id Dem event Id
 **/
#define NVM_WRONG_BLOCK_DEM_EVENT_ID   [!//
[!IF "node:exists(NvmDemEventParameterRefs/NVM_E_WRONG_BLOCK_ID)"!][!//
[!"num:i(node:ref(NvmDemEventParameterRefs/NVM_E_WRONG_BLOCK_ID)/DemEventId)"!]U
[!ELSE!][!//
NVM_INVALID_DEM_EVENTID
[!ENDIF!][!//
#if (defined NVM_PROVIDE_LEGACY_SYMBOLIC_NAMES)

[!IF "NvMCommon/NvMEnableLegacySymbolicNames = 'true'"!][!//
#if (defined NvM_ENABLE_ASR3_LEGACY_NAMES)
#error NvM_ENABLE_ASR3_LEGACY_NAMES is already defined
#endif
/** \brief Enable exporting Legacy Symbolic Names without prefix for AUTOSAR version <= 3.1 rev4. */
#define NvM_ENABLE_ASR3_LEGACY_NAMES 1U
[!ENDIF!]
#endif /* NVM_PROVIDE_LEGACY_SYMBOLIC_NAMES */
#if (defined NVM_BLOCK_MULTI)
#error NVM_BLOCK_MULTI is already defined
#endif
/** \brief Symbolic name for Block 0.
 **/
#define NVM_BLOCK_MULTI                            0U

[!LOOP "NvMBlockDescriptor/*"!][!//
#if (defined NvMConf_NvMBlockDescriptor_[!"name(.)"!])
#error NvMConf_NvMBlockDescriptor_[!"name(.)"!] already defined
#endif
/** \brief Symbolic name for Block [!"NvMNvramBlockIdentifier"!] */
#define NvMConf_NvMBlockDescriptor_[!"name(.)"!]   [!"NvMNvramBlockIdentifier"!]U


#if (defined NVM_PROVIDE_LEGACY_SYMBOLIC_NAMES)
#if (defined NvM_ENABLE_ASR3_LEGACY_NAMES)
    
#if (defined [!"name(.)"!])
#error [!"name(.)"!] already defined
#endif
/** \brief Export symbolic name value without prefix (AUTOSAR version <= 3.1 rev4) */
#define [!"name(.)"!]   [!"NvMNvramBlockIdentifier"!]U
#endif /* NvM_ENABLE_ASR3_LEGACY_NAMES */


#if (defined NvM_[!"name(.)"!])
#error NvM_[!"name(.)"!] already defined
#endif
/** \brief Export symbolic name value with module abbreviation as prefix
 ** only (3.1 rev4 < AUTOSAR version <= AUTOSAR 4.0 rev2) */
#define NvM_[!"name(.)"!]   [!"NvMNvramBlockIdentifier"!]U
#endif /* NVM_PROVIDE_LEGACY_SYMBOLIC_NAMES */

[!ENDLOOP!][!//
[!//
#if (defined NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS)
#error NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS is already defined
#endif
/** \brief Defines the number of all configured blocks including the reserved Block 0.
 **/
#define NVM_TOTAL_NUMBER_OF_NVRAM_BLOCKS [!//
[!"num:i(count(NvMBlockDescriptor/*) + 1)"!]U

#if (defined NVM_BLOCK_ID_MAX)
#error NVM_BLOCK_ID_MAX is already defined
#endif
/** \brief Defines the number of all configured blocks including the reserved Block 0.
 **/
#define NVM_BLOCK_ID_MAX [!//
[!"num:max(NvMBlockDescriptor/*/NvMNvramBlockIdentifier)"!]U

[!IF "NvMCommon/NvMExportBlockLengths = 'true'"!]
[!LOOP "NvMBlockDescriptor/*"!][!//
#if (defined NvMConf_[!"name(.)"!]_Length)
#error NvMConf_[!"name(.)"!]_Length already defined
#endif
/** \brief Export length of Block [!"NvMNvramBlockIdentifier"!] using symbolic name */
#define NvMConf_[!"name(.)"!]_Length    [!"NvMNvBlockLength"!]U

[!ENDLOOP!][!//
[!ENDIF!]

#if (defined NVM_JOB_PRIORITIZATION)
#error NVM_JOB_PRIORITIZATION is already defined
#endif
/** \brief Defines whether the immediate priority queue is used.
 **
 **        Range:
 **        STD_ON  : Immediate priority queue is used.
 **        STD_OFF : Immediate priority queue is not usesd.
 **/
#define NVM_JOB_PRIORITIZATION           [!//
[!IF "NvMCommon/NvMJobPrioritization = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_INTERNAL_BUFFER_SIZE)
#error NVM_INTERNAL_BUFFER_SIZE is already defined
#endif
/** \brief Defines the size of the internal data buffer
 **/
#define NVM_INTERNAL_BUFFER_SIZE         [!//
[!"num:i($maxBlockSize)"!]U

#if (defined NVM_MIRROR_SIZE)
#error NVM_MIRROR_SIZE is already defined
#endif
/* !LINKSTO NVM512,1, NVM513,1 */
/** \brief Defines the size of the NvM internal mirror for explicit synchronization
 **/
#define NVM_MIRROR_SIZE                  [!//
[!IF "count(NvMBlockDescriptor/*[NvMBlockUseSyncMechanism = 'true']) > 0"!][!//
[!"num:i($maxBlockSize)"!]U[!//
[!ELSE!][!//
0U[!//
[!ENDIF!][!//

#if (defined NVM_RAMBLOCK_CRC_BUFFER_SIZE)
#error NVM_RAMBLOCK_CRC_BUFFER_SIZE is already defined
#endif
/** \brief Defines the size of the buffer for storing RAM block CRC values.
 **/
#define NVM_RAMBLOCK_CRC_BUFFER_SIZE     [!//
[!"num:i($CrcBufSize)"!]U

#if (defined NVM_NVBLOCK_CRC_BUFFER_SIZE)
#error NVM_NVBLOCK_CRC_BUFFER_SIZE is already defined
#endif
/** \brief Defines the size of the buffer for storing NV block CRC values.
 **/
#define NVM_NVBLOCK_CRC_BUFFER_SIZE     [!//
[!"num:i($NvCrcBufSize)"!]U

#if (defined NVM_STORED_CRC_SIZE)
#error NVM_STORED_CRC_SIZE is already defined
#endif
/** \brief Defines the size of the buffer for storing CRC values for redundant block.
 **/
#define NVM_STORED_CRC_SIZE              [!//
[!"num:i($maxCrcSize)"!]U

#if (defined NVM_CRYPTO_TIMEOUT)
#error NVM_CRYPTO_TIMEOUT is already defined
#endif
/** \brief Defines the maximum number of calls to the crypto hook for a single block.
**         This is currently hardcoded but is need as a reference to unblock the memory stack in case
**         crypto stack get's stuck.
 **/
#define NVM_CRYPTO_TIMEOUT  1000U

#if (defined NVM_SIZE_STANDARD_JOB_QUEUE)
#error NVM_SIZE_STANDARD_JOB_QUEUE is already defined
#endif
/** \brief The size of the standard job queue.
 **        One entry is reserved for WriteAll or ReadAll or ValidateAll requests,
 **        so the configured value is incremented by one.
 **/
#define NVM_SIZE_STANDARD_JOB_QUEUE      [!//
[!"num:i(NvMCommon/NvMSizeStandardJobQueue + 1)"!]U

#if (defined NVM_SIZE_IMMEDIATE_JOB_QUEUE)
#error NVM_SIZE_IMMEDIATE_JOB_QUEUE is already defined
#endif
/** \brief The size of the immediate job queue.
 **/
#define NVM_SIZE_IMMEDIATE_JOB_QUEUE     [!//
[!IF "node:exists(NvMCommon/NvMSizeImmediateJobQueue)"!][!//
[!"num:i(NvMCommon/NvMSizeImmediateJobQueue)"!][!ELSE!]0[!ENDIF!]U

#if (defined NVM_NUM_USED_DEVICES)
#error NVM_NUM_USED_DEVICES is already defined
#endif
/** \brief Number of devices configured in the lower layers that are accessed thru NvM.
 **/
#define NVM_NUM_USED_DEVICES [!//
[!"num:inttohex($NVM_NrOfDevices,2)"!]U

#if (defined NVM_CONFIGURATIONID_SIZE)
#error NVM_CONFIGURATIONID_SIZE is already defined
#endif
#if (defined NVM_CONFIGURATIONID_PLUSCRC_SIZE)
#error NVM_CONFIGURATIONID_PLUSCRC_SIZE is already defined
#endif
/** \brief The RAM data buffer for the configuration Id shall be defined
 **        according the following rules:
 **
 **        if (for block 1:    NvMBlockUseCrc     == TRUE
 **                   && NvMCalcRamBlockCrc == TRUE
 **                   && NvMBlockCrcType    == NVM_CRC_TYPE_8
 **           )
 **        then   'VAR(uint16,NVM_VAR)        NvM_ConfigurationId[2];'
 **        else
 **        if (for block 1:    NvMBlockUseCrc     == TRUE
 **                   && NvMCalcRamBlockCrc == TRUE
 **                   && NvMBlockCrcType    == NVM_CRC_TYPE_16
 **           )
 **        then   'VAR(uint16,NVM_VAR)        NvM_ConfigurationId[2];'
 **        else
 **        if (for block 1:    NvMBlockUseCrc     == TRUE
 **                          && NvMCalcRamBlockCrc == TRUE
 **                          && NvMBlockCrcType    == NVM_CRC_TYPE_32
 **           )
 **        then  'VAR(uint16,NVM_VAR)  NvM_ConfigurationId[3];'
 **        else  'VAR(uint16,NVM_VAR) NvM_ConfigurationId[1];'
 **/[!/*
*/!][!LOOP "as:modconf('NvM')[1]/NvMBlockDescriptor/*"!][!/*
 */!][!IF "NvMNvramBlockIdentifier = 1"!][!/*
   */!][!IF "(node:exists(NvMBlockCrcType)) and (NvMBlockUseCrc = 'true')"!][!/*
     */!][!IF "NvMBlockCrcType = 'NVM_CRC8'"!][!/*
            */!][!VAR "CRCSize" = "1"!][!/*
            8
     */!][!ELSEIF "NvMBlockCrcType = 'NVM_CRC16'"!][!/*
            */!][!VAR "CRCSize" = "2"!][!/*
            16
     */!][!ELSEIF "NvMBlockCrcType = 'NVM_CRC32'"!][!/*
            */!][!VAR "CRCSize" = "4"!][!/*
            32
     */!][!ELSE!][!/*
            */!][!VAR "CRCSize" = "0"!][!/*
            0
     */!][!ENDIF!][!/*
   */!][!ELSE!][!/*
        */!][!VAR "CRCSize" = "0"!][!/*
    */!][!ENDIF!][!/*
    */!][!VAR "Block1RamSize" = "num:i(NvMNvBlockLength + $CRCSize)"!][!/*
    */!][!VAR "Block1Size" = "num:i(NvMNvBlockLength)"!][!/*
  */!][!ENDIF!][!/*
*/!][!ENDLOOP!]

/** \brief Defines the size in Bytes of the compiled configuration Id.
 **/
#define NVM_CONFIGURATIONID_SIZE        [!//
[!"num:i($Block1Size)"!]U

/** \brief Defines the RamBlock size of the Block1(ConfigurationId block).
 **/
#define NVM_CONFIGURATIONID_PLUSCRC_SIZE        [!//
[!"num:i($Block1RamSize)"!]U

#if (defined NVM_CRC_NUM_OF_BYTES)
#error NVM_CRC_NUM_OF_BYTES is already defined
#endif
/** \brief Defines the maximum number of user data bytes for which the CRC
 **        calculation is not interrupted.
 **/
#define NVM_CRC_NUM_OF_BYTES             [!//
[!"num:i(NvMCommon/NvMCrcNumOfBytes)"!]U

#if (defined NVM_DATASET_SELECTION_BITS)
#error NVM_DATASET_SELECTION_BITS is already defined
#endif
/** \brief Defines the number of bits set aside for data set selection
 **/
#define NVM_DATASET_SELECTION_BITS       [!//
[!"num:i(NvMCommon/NvMDatasetSelectionBits)"!]U

#if (defined NVM_REPEAT_MIRROR_OPERATIONS)
#error NVM_REPEAT_MIRROR_OPERATIONS is already defined
#endif
/** \brief Defines the number of retries to let the application copy data to or from
 **        the NvM module's mirror before postponing the current job.
 **/
#define NVM_REPEAT_MIRROR_OPERATIONS     [!//
[!"num:i(NvMCommon/NvMRepeatMirrorOperations)"!]U

#if (defined NVM_DRV_MODE_SWITCH)
#error NVM_DRV_MODE_SWITCH is already defined
#endif
/** \brief determines if NvM_WriteAll() and NvM_ReadAll() must switch to fast mode.
 **/
#define NVM_DRV_MODE_SWITCH              [!//
[!IF "NvMCommon/NvMDrvModeSwitch = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS)
#error NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMResistantToChangedSw set to false.
 **/
[!VAR "NumberOfNvMResistantToChangedSwBlocks" = "count(NvMBlockDescriptor/*[(NvMResistantToChangedSw = 'false')])"!][!//
#define NVM_NUMBER_OF_NON_RESISTANT_TO_CHANGED_SW_BLOCKS    [!//
[!"num:i($NumberOfNvMResistantToChangedSwBlocks)"!]U

#if (defined NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS)
#error NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMSelectBlockForReadAll set to true.
 **        Block 0 is reserved and independent of this parameter Block 1 is read as part of the NvM_ReadAll.
 **/
[!VAR "NumberOfNvMSelectBlockForReadAllBlocks" = "count(NvMBlockDescriptor/*[(NvMNvramBlockIdentifier > 1) and (node:exists(NvMSelectBlockForReadAll)) and (NvMSelectBlockForReadAll = 'true')])"!][!//
#define NVM_NUMBER_OF_SELECT_BLOCK_FOR_READALL_BLOCKS   [!//
[!"num:i($NumberOfNvMSelectBlockForReadAllBlocks)"!]U

#if (defined NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS)
#error NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMSelectBlockForFirstInitAll set to true.
 **/
[!VAR "NumberOfNvMSelectBlockForFirstInitAllBlocks" = "count(NvMBlockDescriptor/*[(NvMNvramBlockIdentifier > 1) and (node:exists(NvMSelectBlockForFirstInitAll)) and (NvMSelectBlockForFirstInitAll = 'true')])"!][!//
#define NVM_NUMBER_OF_SELECT_BLOCK_FOR_FIRSTINITALL_BLOCKS   [!//
[!"num:i($NumberOfNvMSelectBlockForFirstInitAllBlocks)"!]U

#if (defined NVM_NUMBER_OF_SELECT_BLOCK_FOR_WRITEALL)
#error NVM_NUMBER_OF_SELECT_BLOCK_FOR_WRITEALL is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMSelectBlockForWriteAll set to true.
  **/
[!VAR "NumberOfWriteAllBlocks" =  "count(NvMBlockDescriptor/*[(node:exists(NvMSelectBlockForWriteAll)) and (NvMSelectBlockForWriteAll = 'true')])"!][!//
#define NVM_NUMBER_OF_SELECT_BLOCK_FOR_WRITEALL   [!//
[!"num:i($NumberOfWriteAllBlocks)"!]U

#if (defined NVM_NUMBER_OF_SELECT_BLOCK_FOR_VALIDATEALL)
#error NVM_NUMBER_OF_SELECT_BLOCK_FOR_VALIDATEALL is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMBlockUseAutoValidation set to true.
 **/
[!VAR "NumberOfValidateAllBlocks" =  "count(NvMBlockDescriptor/*[(node:exists(NvMBlockUseAutoValidation)) and (NvMBlockUseAutoValidation = 'true')])"!][!//
#define NVM_NUMBER_OF_SELECT_BLOCK_FOR_VALIDATEALL   [!//
[!"num:i($NumberOfValidateAllBlocks)"!]U

#if (defined NVM_NUMBER_OF_DATASET_BLOCKS)
#error NVM_NUMBER_OF_DATASET_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMBlockManagementType
 **        set to NVM_BLOCK_DATASET.
 **/
[!VAR "NumberOfNvMDatasetBlocks" = "count(NvMBlockDescriptor/*[(NvMBlockManagementType  = 'NVM_BLOCK_DATASET')])"!][!//
#define NVM_NUMBER_OF_DATASET_BLOCKS                    [!//
[!"num:i($NumberOfNvMDatasetBlocks)"!]U

#if (defined NVM_NUMBER_OF_INIT_BLOCK_CALLBACK)
#error NVM_NUMBER_OF_INIT_BLOCK_CALLBACK is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMInitBlockCallback set to not null
 **        NvMProvideRteInitBlockPort is set to true.
 **/
[!VAR "NumberOfNvMInitBlockCallback" = "count(NvMBlockDescriptor/*[(node:exists(NvMInitBlockCallback)) and (NvMInitBlockCallback  != '') and (NvMInitBlockCallback  != 'NULL_PTR')])"!][!//
[!VAR "NumberOfNvMInitBlockCallback" =  "$NumberOfNvMInitBlockCallback + count(NvMBlockDescriptor/*[(NvMProvideRteInitBlockPort = 'true')])"!][!//
#define NVM_NUMBER_OF_INIT_BLOCK_CALLBACK               [!//
[!"num:i($NumberOfNvMInitBlockCallback)"!]U

#if (defined NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK)
#error NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMSingleBlockCallback set to not null.
 **/
[!VAR "NumberOfNvMSingleBlockCallback" = "count(NvMBlockDescriptor/*[(node:exists(NvMSingleBlockCallback)) and (NvMSingleBlockCallback  != '' )])"!][!//
[!VAR "NumberOfNvMSingleBlockCallback" = "$NumberOfNvMSingleBlockCallback + count(NvMBlockDescriptor/*[(NvMProvideRteJobFinishedPort = 'true')])"!][!//
#define NVM_NUMBER_OF_SINGLE_BLOCK_CALLBACK             [!//
[!"num:i($NumberOfNvMSingleBlockCallback)"!]U

#if (defined NVM_NUMBER_OF_CALC_CRC8_BLOCKS)
#error NVM_NUMBER_OF_CALC_CRC8_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMBlockUseCrc set to true and
 **        NvMBlockCrcType set to NVM_CRC8.
 **/
[!VAR "NumberOfCalcCrc8Blocks" = "count(NvMBlockDescriptor/*[(NvMBlockUseCrc = 'true') and (node:exists(NvMBlockCrcType)) and (NvMBlockCrcType = 'NVM_CRC8')])"!][!//
#define NVM_NUMBER_OF_CALC_CRC8_BLOCKS                 [!//
[!"num:i($NumberOfCalcCrc8Blocks)"!]

#if (defined NVM_NUMBER_OF_CALC_CRC16_BLOCKS)
#error NVM_NUMBER_OF_CALC_CRC16_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMBlockUseCrc set to true and
 **        NvMBlockCrcType set to NVM_CRC16.
 **/
[!VAR "NumberOfCalcCrc16Blocks" = "count(NvMBlockDescriptor/*[(NvMBlockUseCrc = 'true') and (node:exists(NvMBlockCrcType)) and (NvMBlockCrcType = 'NVM_CRC16')])"!][!//
#define NVM_NUMBER_OF_CALC_CRC16_BLOCKS                 [!//
[!"num:i($NumberOfCalcCrc16Blocks)"!]

#if (defined NVM_NUMBER_OF_CALC_CRC32_BLOCKS)
#error NVM_NUMBER_OF_CALC_CRC32_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMBlockUseCrc set to true and
 **        NvMBlockCrcType set to NVM_CRC32.
 **/
[!VAR "NumberOfCalcCrc32Blocks" = "count(NvMBlockDescriptor/*[(NvMBlockUseCrc = 'true') and (node:exists(NvMBlockCrcType)) and (NvMBlockCrcType = 'NVM_CRC32')])"!][!//
#define NVM_NUMBER_OF_CALC_CRC32_BLOCKS                 [!//
[!"num:i($NumberOfCalcCrc32Blocks)"!]

#if (defined NVM_NUMBER_OF_CRC_BLOCKS)
#error NVM_NUMBER_OF_CRC_BLOCKS is already defined
#endif
/** \brief Defines the number of NVRAM Blocks which have
 **        the configuration parameter NvMBlockUseCrc set to true and
 **        NvMBlockCrcType set to NVM_CRC32.
 **/
[!VAR "NumberOfCalcCrcBlocks" = "num:i($NumberOfCalcCrc32Blocks) + num:i($NumberOfCalcCrc16Blocks) + num:i($NumberOfCalcCrc8Blocks)"!][!//
#define NVM_NUMBER_OF_CRC_BLOCKS                 [!//
[!"num:i($NumberOfCalcCrcBlocks)"!]

#if (defined NVM_CRC_COMP_MECH)
#error NVM_CRC_COMP_MECH is already defined
#endif
/** \brief Defines whether CRC Compare mechanism is turned ON for any block
 **        by checking the number of NVRAM Blocks having NvMBlockUseCRCCompMechanism set to true.
 **/
#define NVM_CRC_COMP_MECH                          [!//
[!IF "count(NvMBlockDescriptor/*[(NvMBlockUseCRCCompMechanism = 'true')]) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO)
#error NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO is already defined
#endif
/** \brief Defines whether BswM job status info is enabled/disabled
 **        by checking NvMBswMMultiBlockJobStatusInformation set to true.
 **/
#define NVM_BSWM_MULTI_BLOCK_JOB_STATUS_INFO            [!//
[!IF "NvMCommon/NvMBswMMultiBlockJobStatusInformation = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_BSWM_BLOCK_STATUS_INFO)
#error NVM_BSWM_BLOCK_STATUS_INFO is already defined
#endif
/** \brief Defines whether BswM block status info is enabled/disabled
 **        by checking the number of NVRAM blocks having
 **        NvMBswMBlockStatusInformation set to true.
 **/
#define NVM_BSWM_BLOCK_STATUS_INFO                      [!//
[!IF "count(NvMBlockDescriptor/*[(NvMBswMBlockStatusInformation = 'true')]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_WRITE_VERIFICATION)
#error NVM_WRITE_VERIFICATION is already defined
#endif
/** \brief Defines whether Write verification is turned on or not
 **        by checking the number of NVRAM Blocks having NvMWriteVerification set to true.
 **/
#define NVM_WRITE_VERIFICATION                          [!//
[!IF "count(NvMBlockDescriptor/*[(NvMWriteVerification = 'true')]) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_WRITE_VERIFICATION_BUFFER_SIZE)
#error NVM_WRITE_VERIFICATION_BUFFER_SIZE is already defined
#endif
/** \brief Defines the size of the Write verification buffer.
 **       This is used for normal post writing verification and also for pre writing verification.
 **/
#define NVM_WRITE_VERIFICATION_BUFFER_SIZE              [!//
[!"num:i($maxVerificationSize)"!]

#if (defined NVM_PRE_WRITE_DATA_COMP)
#error NVM_PRE_WRITE_DATA_COMP is already defined
#endif
/** \brief Defines whether Pre Write Data Comparison is turned on or not
 **        by checking the number of NVRAM Blocks having NvMPreWriteDataComp set to true.
 **/
#define NVM_PRE_WRITE_DATA_COMP                        [!//
[!IF "count(NvMBlockDescriptor/*[(NvMPreWriteDataComp = 'true')]) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_STATIC_BLOCKID_CHECK)
#error NVM_STATIC_BLOCKID_CHECK is already defined
#endif
/** \brief Defines whether Static Block Id check is turned on or not
 **        by checking the number of NVRAM Blocks having NvMStaticBlockIDCheck set to true.
 **/
#define NVM_STATIC_BLOCKID_CHECK                        [!//
[!IF "count(NvMBlockDescriptor/*[(NvMStaticBlockIDCheck = 'true')]) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_SYNC_MECHANISM)
#error NVM_SYNC_MECHANISM is already defined
#endif
/** \brief Defines whether Explicit Synchronization is turned on or not
 **        by checking the number of NVRAM Blocks having NvMBlockUseSyncMechanism set to true.
 **/
#define NVM_SYNC_MECHANISM                              [!//
[!IF "count(NvMBlockDescriptor/*[(NvMBlockUseSyncMechanism = 'true')]) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS)
#error NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS is already defined
#endif
/** \brief Defines whether any block is configured with immediate priority
 **        by checking the number of NVRAM Blocks having NvMBlockJobPriority set to zero.
 **/
#define NVM_NUMBER_OF_IMMEDIATE_PRIORITY_BLOCKS         [!//
[!"num:i(count(NvMBlockDescriptor/*[(NvMBlockJobPriority = 0)]))"!]U

#if (defined NVM_WRITE_BLOCK_ONCE)
#error NVM_WRITE_BLOCK_ONCE is already defined
#endif
/** \brief Defines whether any block is configured with write block once
 **        by checking the number of NVRAM Blocks having NvMWriteBlockOnce set to zero.
 **/
#define NVM_WRITE_BLOCK_ONCE                            [!//
[!IF "count(NvMBlockDescriptor/*[(NvMWriteBlockOnce = 'true')]) != 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_NUMBER_OF_WRITE_RETRY_BLOCKS)
#error NVM_NUMBER_OF_WRITE_RETRY_BLOCKS is already defined
#endif
/** \brief Defines whether any block is configured with write retry
 **        by checking the number of NVRAM Blocks having NvMMaxNumOfWriteRetries set to non-zero.
 **/
#define NVM_NUMBER_OF_WRITE_RETRY_BLOCKS                [!//
[!"num:i(count(NvMBlockDescriptor/*[(NvMMaxNumOfWriteRetries > 0)]))"!]U

#if (defined NVM_NUMBER_OF_READ_RETRY_BLOCKS)
#error NVM_NUMBER_OF_READ_RETRY_BLOCKS is already defined
#endif
/** \brief Defines whether any block is configured with read retry
 **        by checking the number of NVRAM Blocks having NvMMaxNumOfReadRetries set to non-zero.
 **/
#define NVM_NUMBER_OF_READ_RETRY_BLOCKS                 [!//
[!"num:i(count(NvMBlockDescriptor/*[(NvMMaxNumOfReadRetries > 0)]))"!]U

#if (defined NVM_API_CONFIG_CLASS_1)
#error NVM_API_CONFIG_CLASS_1 is already defined
#endif
/** \brief Defines the NvM Api config class 1.
 **/
#define NVM_API_CONFIG_CLASS_1                          0U

#if (defined NVM_API_CONFIG_CLASS_2)
#error NVM_API_CONFIG_CLASS_2 is already defined
#endif
/** \brief Defines the NvM Api config class 2.
 **/
#define NVM_API_CONFIG_CLASS_2                          1U

#if (defined NVM_API_CONFIG_CLASS_3)
#error NVM_API_CONFIG_CLASS_3 is already defined
#endif
/** \brief Defines the NvM Api config class 3.
 **/
#define NVM_API_CONFIG_CLASS_3                          2U

#if (defined NVM_API_CONFIG_CLASS)
#error NVM_API_CONFIG_CLASS is already defined
#endif
/** \brief Defines API config class
 **        NVM_API_CONFIG_CLASS_3 - all API functions are available
 **        NVM_API_CONFIG_CLASS_2 - an intermediate set API functions is available
 **        NVM_API_CONFIG_CLASS_1 - a minimum set of API functions is available
 **/
#define NVM_API_CONFIG_CLASS                            [!//
[!"NvMCommon/NvMApiConfigClass"!]

#if (defined NVM_POLLING_MODE)
#error NVM_POLLING_MODE is already defined
#endif
/** \brief Preprocessor switch to enable/disable the polling mode in the NVRAM Manager
 **        and at the same time disable/enable the callback functions useable by lower layers.
 **
 **        STD_ON:  Polling mode enabled/callbacks disabled
 **        STD_OFF: Polling mode disabled/callbacks enabled
 **/
#define NVM_POLLING_MODE                 [!//
[!IF "NvMCommon/NvMPollingMode ='true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_DYNAMIC_CONFIGURATION)
#error NVM_DYNAMIC_CONFIGURATION is already defined
#endif
/** \brief Preprocessor switch to enable the dynamic configuration management handling
 **        by the NvM_ReadAll request.
 **        STD_ON: Dynamic configuration management handling enabled.
 **        STD_OFF: Dynamic configuration management handling disabled.
 **/
#define NVM_DYNAMIC_CONFIGURATION                 [!//
[!IF "NvMCommon/NvMDynamicConfiguration = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_USE_COMPILED_CONFIGID)
#error NVM_USE_COMPILED_CONFIGID is already defined
#endif
/** \brief Preprocessor switch to enable the usage of configuration ID block content for
 **        configuration change detection.
 **        STD_ON: The compiled configuration id is compared to the content of block 1.
 **        STD_OFF: The user configured a callout for configuration change detection.
 **/
#define NVM_USE_COMPILED_CONFIGID                 [!//
[!IF "(node:exists(NvMCommon/NvMSoftwareChangeCallout)) and (NvMCommon/NvMSoftwareChangeCallout != '')"!]STD_OFF[!ELSE!]STD_ON[!ENDIF!]

#if (defined NVM_RECOVERY_ON_REQUEST)
#error NVM_RECOVERY_ON_REQUEST is already defined
#endif
/** \brief Defines the NvM recovery of redundant blocks on request
 **/
#define NVM_RECOVERY_ON_REQUEST                         0U

#if (defined NVM_AUTOMATIC_RECOVERY)
#error NVM_AUTOMATIC_RECOVERY is already defined
#endif
/** \brief Defines the NvM automatic recovery of redundant blocks
 **/
#define NVM_AUTOMATIC_RECOVERY                          1U

#if (defined NVM_REDUNDANT_BLOCK_RECOVERY)
#error NVM_REDUNDANT_BLOCK_RECOVERY is already defined
#endif
/** \brief Defines NvM behavior in case loss of redundancy is detected for redundant blocks
 **        NVM_RECOVERY_ON_REQUEST - loss of recovery not visible to the user
 **        NVM_AUTOMATIC_RECOVERY - loss of recovery transparent to the user
 **/
#define NVM_REDUNDANT_BLOCK_RECOVERY                    [!//
[!"NvMCommon/NvMRedundantRecovery"!]

#if (defined NVM_READ_BLOCK_HOOK)
#error NVM_READ_BLOCK_HOOK is already defined
#endif
/** \brief Hook functions macros for ReadBlock and WriteBlock. They are called after
 **        the NvM calls the MemIf_Read and before it calls the NvM MemIf_Write
 **        function respectively.
 **/
#define NVM_READ_BLOCK_HOOK(BlockNum, RamBlockDataAddress, BlockLength) \
[!IF "NvMCommon/NvMReadBlockHook = 'true'"!][!//
  NvM_ReadBlockHook(BlockNum, RamBlockDataAddress, BlockLength)
[!ELSE!][!//
  do {} while (0)
[!ENDIF!][!//

#if (defined NVM_WRITE_BLOCK_HOOK)
#error NVM_WRITE_BLOCK_HOOK is already defined
#endif
#define NVM_WRITE_BLOCK_HOOK(BlockNum, RamBlockDataAddress, BlockLength) \
[!IF "NvMCommon/NvMWriteBlockHook = 'true'"!][!//
  NvM_WriteBlockHook(BlockNum, RamBlockDataAddress, BlockLength)
[!ELSE!][!//
  do {} while (0)
[!ENDIF!][!//

#if (defined NVM_MULTI_BLOCK_CALLBACK)
#error NVM_MULTI_BLOCK_CALLBACK is already defined
#endif
/** \brief Multi block callback function macro. It is called
 **        if the NvM multi request NvM_WriteAll() or NvM_ReadAll() or NvM_ValidateAll is
 **        finished.
 **/
#define NVM_MULTI_BLOCK_CALLBACK(sid,errorStatus) \
[!IF "(node:exists(NvMCommon/NvMMultiBlockCallback)) and (NvMCommon/NvMMultiBlockCallback != '')"!][!//
  [!"NvMCommon/NvMMultiBlockCallback"!]((sid),(errorStatus))
[!ELSE!][!//
  do { } while (FALSE)
[!ENDIF!][!//

#if (defined NVM_SOFTWARE_CHANGE_CALLOUT)
#error NVM_SOFTWARE_CHANGE_CALLOUT is already defined
#endif
/** \brief Software change function macro. It is called
 **        in order to obtain the user Configuration Id.
 **/
[!IF "(node:exists(NvMCommon/NvMSoftwareChangeCallout)) and (NvMCommon/NvMSoftwareChangeCallout != '')"!][!//
#define NVM_SOFTWARE_CHANGE_CALLOUT() \
  [!"NvMCommon/NvMSoftwareChangeCallout"!]()
[!ENDIF!][!//

[!IF "(node:exists(ReportToDem/NvMUserCalloutFunctionProductionErrors)) and (ReportToDem/NvMUserCalloutFunctionProductionErrors != '') and (ReportToDem/NvMUserCalloutFunctionProductionErrors != 'NULL_PTR')"!][!//

#if (defined NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT)
#error NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT is already defined
#endif
/** \brief Callout for production errors.
 **/
#define NVM_PRODUCTION_ERRORS_FUNCTION_CALLOUT(BlockId, ApiId, ErrorId) [!//
  [!"ReportToDem/NvMUserCalloutFunctionProductionErrors"!]((BlockId),(ApiId),(ErrorId))
[!ENDIF!][!//

[!IF "(node:exists(ReportToDem/NvMUserCalloutFunctionPassedProductionErrors)) and (ReportToDem/NvMUserCalloutFunctionPassedProductionErrors!='') and (ReportToDem/NvMUserCalloutFunctionPassedProductionErrors != 'NULL_PTR')"!][!//

#if (defined NVM_PRODUCTION_PASSED_ERRORS_FUNCTION_CALLOUT)
#error NVM_PRODUCTION_PASSED_ERRORS_FUNCTION_CALLOUT is already defined
#endif
/** \brief Callout for production errors.
 **/
#define NVM_PRODUCTION_PASSED_ERRORS_FUNCTION_CALLOUT(BlockId, ApiId, ErrorId) [!//
  [!"ReportToDem/NvMUserCalloutFunctionPassedProductionErrors"!]((BlockId),(ApiId),(ErrorId))
[!ENDIF!][!//

[!IF "(node:exists(MultiCoreCallout/NvMReadBlockCallout)) and (MultiCoreCallout/NvMReadBlockCallout != '')"!][!//
#if (defined NVM_READBLOCK_CALLOUT)
#error NVM_READBLOCK_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_ReadBlock() API.
 **/
#define NVM_READBLOCK_CALLOUT(BlockId, NvM_DstPtr) [!//
  [!"MultiCoreCallout/NvMReadBlockCallout"!]((BlockId),(NvM_DstPtr))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMWriteBlockCallout)) and (MultiCoreCallout/NvMWriteBlockCallout != '')"!][!//

#if (defined NVM_WRITEBLOCK_CALLOUT)
#error NVM_WRITEBLOCK_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_WriteBlock() API.
 **/
#define NVM_WRITEBLOCK_CALLOUT(BlockId, NvM_DstPtr) [!//
  [!"MultiCoreCallout/NvMWriteBlockCallout"!]((BlockId),(NvM_DstPtr))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMRestoreBlockDefaultsCallout)) and (MultiCoreCallout/NvMRestoreBlockDefaultsCallout != '')"!][!//

#if (defined NVM_RESTOREBLOCKDEFAULTS_CALLOUT)
#error NVM_RESTOREBLOCKDEFAULTS_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_RestoreBlockDefaults() API.
 **/
#define NVM_RESTOREBLOCKDEFAULTS_CALLOUT(BlockId, NvM_DstPtr) [!//
  [!"MultiCoreCallout/NvMRestoreBlockDefaultsCallout"!]((BlockId),(NvM_DstPtr))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMReadPRAMBlockCallout)) and (MultiCoreCallout/NvMReadPRAMBlockCallout != '')"!][!//

#if (defined NVM_READPRAMBLOCK_CALLOUT)
#error NVM_READPRAMBLOCK_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_ReadPRAMBlock() API.
 **/
#define NVM_READPRAMBLOCK_CALLOUT(BlockId) [!//
  [!"MultiCoreCallout/NvMReadPRAMBlockCallout"!]((BlockId))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMWritePRAMBlockCallout)) and (MultiCoreCallout/NvMWritePRAMBlockCallout != '')"!][!//

#if (defined NVM_WRITEPRAMBLOCK_CALLOUT)
#error NVM_WRITEPRAMBLOCK_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_WritePRAMBlock() API.
 **/
#define NVM_WRITEPRAMBLOCK_CALLOUT(BlockId) [!//
  [!"MultiCoreCallout/NvMWritePRAMBlockCallout"!]((BlockId))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMRestorePRAMBlockDefaultsCallout)) and (MultiCoreCallout/NvMRestorePRAMBlockDefaultsCallout != '')"!][!//

#if (defined NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT)
#error NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_RestorePRAMBlockDefaults() API.
 **/
#define NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT(BlockId) [!//
  [!"MultiCoreCallout/NvMRestorePRAMBlockDefaultsCallout"!]((BlockId))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMInvalidateNvBlockCallout)) and (MultiCoreCallout/NvMInvalidateNvBlockCallout != '')"!][!//

#if (defined NVM_INVALIDATENVBLOCK_CALLOUT)
#error NVM_INVALIDATENVBLOCK_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_InvalidateNvBlock() API.
 **/
#define NVM_INVALIDATENVBLOCK_CALLOUT(BlockId) [!//
  [!"MultiCoreCallout/NvMInvalidateNvBlockCallout"!]((BlockId))
[!ENDIF!][!//
[!IF "(node:exists(MultiCoreCallout/NvMEraseNvBlockCallout)) and (MultiCoreCallout/NvMEraseNvBlockCallout != '')"!][!//

#if (defined NVM_ERASENVBLOCK_CALLOUT)
#error NVM_ERASENVBLOCK_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_EraseNvBlock() API.
 **/
#define NVM_ERASENVBLOCK_CALLOUT(BlockId) [!//
  [!"MultiCoreCallout/NvMEraseNvBlockCallout"!]((BlockId))
[!ENDIF!][!//

[!IF "(node:exists(MultiCoreCallout/NvMCancelJobsCallout)) and (MultiCoreCallout/NvMCancelJobsCallout != '')"!][!//

#if (defined NVM_CANCELJOBS_CALLOUT)
#error NVM_CANCELJOBS_CALLOUT is already defined
#endif
/** \brief Callout for multi-core configuration to be called instead of NvM_CancelJobs() API.
 **/
#define NVM_CANCELJOBS_CALLOUT(BlockId) [!//
  [!"MultiCoreCallout/NvMCancelJobsCallout"!]((BlockId))
[!ENDIF!][!//

#if(defined(NVM_READBLOCK_CALLOUT) || defined(NVM_WRITEBLOCK_CALLOUT) || defined(NVM_RESTOREBLOCKDEFAULTS_CALLOUT) ||\
    defined(NVM_READPRAMBLOCK_CALLOUT) || defined(NVM_WRITEPRAMBLOCK_CALLOUT) || defined(NVM_RESTOREPRAMBLOCKDEFAULTS_CALLOUT)||\
    defined(NVM_INVALIDATENVBLOCK_CALLOUT) || defined(NVM_ERASENVBLOCK_CALLOUT) || defined(NVM_CANCELJOBS_CALLOUT))
/** \brief Macro to flag that any NvM multicore callout is enabled.
 **/
#define NVM_MULTICORE_ENABLED
#endif
#if (defined NVM_ERASED_BLOCK_INVALID)
#error NVM_ERASED_BLOCK_INVALID is already defined
#endif
/** \brief Defines the NvM behavior if erased/not found blocks are reported as INVALID
 **/
#define NVM_ERASED_BLOCK_INVALID                        0U

#if (defined NVM_ERASED_BLOCK_INCONSISTENT)
#error NVM_ERASED_BLOCK_INCONSISTENT is already defined
#endif
/** \brief Defines the NvM behavior if erased/not found blocks are reported as INCONSISTENT
 **/
#define NVM_ERASED_BLOCK_INCONSISTENT                   1U

#if (defined NVM_RESULT_ERASED_BLOCKS)
#error NVM_RESULT_ERASED_BLOCKS is already defined
#endif
/** \brief Defines the reported block result from lower layer for erased/not found blocks.
 **
 **        Range:
 **        MEMIF_BLOCK_INVALID  :     Erased/Not found blocks are reported as invalid.
 **        MEMIF_BLOCK_INCONSISTENT : Erased/Not found blocks are reported as inconsistent.
 **/
#define NVM_RESULT_ERASED_BLOCKS         [!//
[!IF "NvMCommon/NvMResultErasedBlocks = 'MEMIF_BLOCK_INVALID'"!][!//
NVM_ERASED_BLOCK_INVALID[!ELSE!]NVM_ERASED_BLOCK_INCONSISTENT[!ENDIF!]

#if (defined NVM_ADMIN_PORT_USED)
#error NVM_ADMIN_PORT_USED is already defined
#endif
/** \brief Rte admin port macro used for declaring NvM_SetBlockProtection()
 **        when Rte usage is enabled but Rte admin port is disabled for all
 **        configured blocks.
 **/
#define NVM_ADMIN_PORT_USED                       [!//
[!IF "count(NvMBlockDescriptor/*[(NvMProvideRteAdminPort = 'true')]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_SERVICE_PORT_USED)
#error NVM_SERVICE_PORT_USED is already defined
#endif
/** \brief Rte service port macro used for declaring NvM_GetErrorStatus()
 **        when Rte usage is enabled but Rte service port is disabled for all
 **        configured blocks.
 **/
#define NVM_SERVICE_PORT_USED                       [!//
[!IF "count(NvMBlockDescriptor/*[(NvMProvideRteServicePort = 'true')]) > 0"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_SERVICE_API_NONE)
#error NVM_SERVICE_API_NONE already defined
#endif
/** \brief Definition that no default service API is set **/
#define NVM_SERVICE_API_NONE   0U

#if (defined NVM_SERVICE_API_ASR32)
#error NVM_SERVICE_API_ASR32 already defined
#endif
/** \brief Definition that AUTOSAR 3.2 service API is set as default **/
#define NVM_SERVICE_API_ASR32   32U

#if (defined NVM_SERVICE_API_ASR40)
#error NVM_SERVICE_API_ASR40 already defined
#endif
/** \brief Definition that AUTOSAR 4.0 service API is set as default **/
#define NVM_SERVICE_API_ASR40   40U

#if (defined NVM_SERVICE_API_ASR42)
#error NVM_SERVICE_API_ASR42 already defined
#endif
/** \brief Definition that AUTOSAR 4.2 service API is set as default **/
#define NVM_SERVICE_API_ASR42   42U

[!VAR "ServiceAPIEnabled" = "node:exists(NvMCommon/NvMServiceAPI)"!] [!//
[!IF "$ServiceAPIEnabled = 'true'"!] [!//
#if (defined NVM_ENABLE_ASR32_SERVICE_API) /* To prevent double declaration */
#error NVM_ENABLE_ASR32_SERVICE_API already defined
#endif /* if (defined NVM_ENABLE_ASR32_SERVICE_API) */
/** \brief Definition whether AUTOSAR 3.2 service API is enabled **/
#define NVM_ENABLE_ASR32_SERVICE_API                   [!//
[!IF "NvMCommon/NvMServiceAPI/NvMEnableASR32ServiceAPI"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_ENABLE_ASR40_SERVICE_API) /* To prevent double declaration */
#error NVM_ENABLE_ASR40_SERVICE_API already defined
#endif /* if (defined NVM_ENABLE_ASR40_SERVICE_API) */
/** \brief Definition whether AUTOSAR 4.0 service API is enabled **/
#define NVM_ENABLE_ASR40_SERVICE_API                   [!//
[!IF "NvMCommon/NvMServiceAPI/NvMEnableASR40ServiceAPI"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_ENABLE_ASR42_SERVICE_API) /* To prevent double declaration */
#error NVM_ENABLE_ASR42_SERVICE_API already defined
#endif /* if (defined NVM_ENABLE_ASR42_SERVICE_API) */
/** \brief Definition whether AUTOSAR 4.2 service API is enabled **/
#define NVM_ENABLE_ASR42_SERVICE_API                   [!//
[!IF "NvMCommon/NvMServiceAPI/NvMEnableASR42ServiceAPI"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_DEFAULT_ASR_SERVICE_API) /* To prevent double declaration */
#error NVM_DEFAULT_ASR_SERVICE_API already defined
#endif /* if (defined NVM_DEFAULT_ASR_SERVICE_API) */
/** \brief Definition of default service API **/
#define NVM_DEFAULT_ASR_SERVICE_API                    [!//
[!IF "NvMCommon/NvMServiceAPI/NvMDefaultASRServiceAPI = 'AUTOSAR_32'"!]NVM_SERVICE_API_ASR32
[!ELSEIF "NvMCommon/NvMServiceAPI/NvMDefaultASRServiceAPI = 'AUTOSAR_40'"!]NVM_SERVICE_API_ASR40
[!ELSEIF "NvMCommon/NvMServiceAPI/NvMDefaultASRServiceAPI = 'AUTOSAR_42'"!]NVM_SERVICE_API_ASR42
[!ELSE!]NVM_SERVICE_API_NONE
[!ENDIF!][!//
[!ELSE!] [!//
#if (defined NVM_ENABLE_ASR32_SERVICE_API) /* To prevent double declaration */
#error NVM_ENABLE_ASR32_SERVICE_API already defined
#endif /* if (defined NVM_ENABLE_ASR32_SERVICE_API) */
/** \brief Definition whether AUTOSAR 3.2 service API is enabled **/
#define NVM_ENABLE_ASR32_SERVICE_API                   STD_OFF

#if (defined NVM_ENABLE_ASR40_SERVICE_API) /* To prevent double declaration */
#error NVM_ENABLE_ASR40_SERVICE_API already defined
#endif /* if (defined NVM_ENABLE_ASR40_SERVICE_API) */
/** \brief Definition whether AUTOSAR 4.0 service API is enabled **/
#define NVM_ENABLE_ASR40_SERVICE_API                  STD_OFF

#if (defined NVM_ENABLE_ASR42_SERVICE_API) /* To prevent double declaration */
#error NVM_ENABLE_ASR42_SERVICE_API already defined
#endif /* if (defined NVM_ENABLE_ASR42_SERVICE_API) */
/** \brief Definition whether AUTOSAR 4.2 service API is enabled **/
#define NVM_ENABLE_ASR42_SERVICE_API                  STD_OFF

#if (defined NVM_DEFAULT_ASR_SERVICE_API) /* To prevent double declaration */
#error NVM_DEFAULT_ASR_SERVICE_API already defined
#endif /* if (defined NVM_DEFAULT_ASR_SERVICE_API) */
/** \brief Definition of default service API **/
#define NVM_DEFAULT_ASR_SERVICE_API                   NVM_SERVICE_API_NONE
[!ENDIF!]

/*================[constants user application section]================*/
[!IF "not(node:exists(NvMCommon/NvMSoftwareChangeCallout)) and (NvMCommon/NvMSoftwareChangeCallout != '')"!]
 #define NVM_START_SEC_CONFIG_DATA_APPL_CONST
 #include <NvM_MemMap.h>

 /** \brief Stores the value of the configuration ID set by the user.
  **/[!/*
 */!][!IF "$Block1Size != 2"!]
extern CONST( uint8,NVM_CONST ) NvM_CompiledConfigurationId[NVM_CONFIGURATIONID_SIZE];
[!ELSE!]
extern CONST(uint16,NVM_APPL_CONST) NvM_CompiledConfigurationId;
[!ENDIF!][!//
#define NVM_STOP_SEC_CONFIG_DATA_APPL_CONST
#include <NvM_MemMap.h>
[!ELSE!]
#define NVM_START_SEC_VAR_INIT_UNSPECIFIED
#include <NvM_MemMap.h>
/** \brief Stores the address of the new Configuration Id
 **/
extern VAR( uint8,NVM_VAR ) NvM_CompiledConfigurationId[NVM_CONFIGURATIONID_SIZE];
#define NVM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <NvM_MemMap.h>
[!ENDIF!]

[!IF "(count(NvMBlockDescriptor/*) != num:max(NvMBlockDescriptor/*/NvMNvramBlockIdentifier))"!]
#define NVM_START_SEC_CONFIG_DATA_16
#include <NvM_MemMap.h>

extern CONST(uint16, NVM_CONST) NvM_RedirBlockId[[!"num:i((num:max(NvMBlockDescriptor/*/NvMNvramBlockIdentifier)) + 1)"!]U];

#define NVM_STOP_SEC_CONFIG_DATA_16
#include <NvM_MemMap.h>
[!ENDIF!]
 
 /*==================[BC type definitions]======================================*/
/**  \brief Defines the data type BC index can take. It can be uint8 or uint16
           depending on the numbers of blocks configured with BC.
 **/
#if (NVM_BC_NR_OF_BLOCKS < 256)
typedef uint8 uintBC;
#else
typedef uint16 uintBC;
#endif

/*================[variables user application data section]================*/

/** \brief Stores the calculated CRC values in RAM
 **/
[!IF "$maxCrcSize=1"!]
#define NVM_START_SEC_VAR_INIT_8
#include <NvM_MemMap.h>
extern VAR(uint8,NVM_APPL_DATA) NvM_CalcCrc_CalcBuffer;
#define NVM_STOP_SEC_VAR_INIT_8
#include <NvM_MemMap.h>
[!ELSEIF "$maxCrcSize=2"!]
#define NVM_START_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
extern VAR(uint16,NVM_APPL_DATA) NvM_CalcCrc_CalcBuffer;
#define NVM_STOP_SEC_VAR_INIT_16
#include <NvM_MemMap.h>
[!ELSE!]
#define NVM_START_SEC_VAR_INIT_32
#include <NvM_MemMap.h>
extern VAR(uint32,NVM_APPL_DATA) NvM_CalcCrc_CalcBuffer;
#define NVM_STOP_SEC_VAR_INIT_32
#include <NvM_MemMap.h>
[!ENDIF!]

/*===============[end of variables user application data section]==========*/

/*==================[external function declarations]========================*/

#define NVM_START_SEC_CODE
#include <NvM_MemMap.h>

[!IF "NvMCommon/NvMReadBlockHook = 'true'"!][!//
/** \brief Hook function for ReadBlock
 **
 **        The NvM_ReadBlockHook() provides a pre-read hook functionality.
 **        After reading the data from the underlying layer, the hook function
 **        is called.
 **
 ** \param[in] BlockNum The block identifier of the currently processed block.
 ** \param[in] RamBlockDataAddress Pointer to a permanent RAM block. If
 **            no permanent RAM block is configured, it returns NULL_PTR
 ** \param[in] BlockLength Length of the currently processed block
 **
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NVM_CODE) NvM_ReadBlockHook
(
  VAR(uint16, AUTOMATIC)                 BlockNum,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) RamBlockDataAddress,
  VAR(uint16, AUTOMATIC)                 BlockLength
);

[!ENDIF!][!//
[!IF "NvMCommon/NvMWriteBlockHook = 'true'"!][!//
/** \brief Hook function for WriteBlock
 **
 **        The NvM_WriteBlockHook() provides a post-write hook functionality.
 **        Before writing the data to the underlying layer, the hook function is
 **        called.
 **
 ** \param[in] BlockNum The block identifier of the currently processed block.
 ** \param[in] RamBlockDataAddress Pointer to a permanent RAM block. If
 **            no permanent RAM block is configured, it returns NULL_PTR
 ** \param[in] BlockLength Length of the currently processed block
 **
 ** \Synchronicity{Synchronous} */
extern FUNC(void, NVM_CODE) NvM_WriteBlockHook
(
  VAR(uint16, AUTOMATIC)                 BlockNum,
  P2VAR(uint8, AUTOMATIC, NVM_APPL_DATA) RamBlockDataAddress,
  VAR(uint16, AUTOMATIC)                 BlockLength
);
[!ENDIF!][!//

#define NVM_STOP_SEC_CODE
#include <NvM_MemMap.h>

/*------------------[Defensive programming]---------------------------------*/
[!SELECT "NvMDefensiveProgramming"!][!//

#if (defined NVM_DEFENSIVE_PROGRAMMING_ENABLED)
#error NVM_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define NVM_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../NvMCommon/NvMDevErrorDetect  = 'true') and (NvMDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_PRECONDITION_ASSERT_ENABLED)
#error NVM_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define NVM_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../NvMCommon/NvMDevErrorDetect  = 'true') and (NvMDefProgEnabled = 'true') and (NvMPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_POSTCONDITION_ASSERT_ENABLED)
#error NVM_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define NVM_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../NvMCommon/NvMDevErrorDetect  = 'true') and (NvMDefProgEnabled = 'true') and (NvMPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_UNREACHABLE_CODE_ASSERT_ENABLED)
#error NVM_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define NVM_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../NvMCommon/NvMDevErrorDetect  = 'true') and (NvMDefProgEnabled = 'true') and (NvMUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_INVARIANT_ASSERT_ENABLED)
#error NVM_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define NVM_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../NvMCommon/NvMDevErrorDetect  = 'true') and (NvMDefProgEnabled = 'true') and (NvMInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined NVM_STATIC_ASSERT_ENABLED)
#error NVM_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define NVM_STATIC_ASSERT_ENABLED           [!//
[!IF "(../NvMCommon/NvMDevErrorDetect  = 'true') and (NvMDefProgEnabled = 'true') and (NvMStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//

/*==================[ end of external function declarations]================*/
#endif /* ifndef NVM_CFG_H */
/*==================[end of file NvM_Cfg.h]==================================*/
