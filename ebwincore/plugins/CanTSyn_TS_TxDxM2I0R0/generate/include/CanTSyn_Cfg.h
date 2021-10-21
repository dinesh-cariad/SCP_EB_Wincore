/**
 * \file
 *
 * \brief AUTOSAR CanTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTSyn.
 *
 * \version 2.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

[!CODE!]
[!AUTOSPACING!]

#ifndef CANTSYN_CFG_H
#define CANTSYN_CFG_H

/*==================[includes]==============================================*/

[!INCLUDE "CanTSyn_PrecompileMacros.m"!][!//

#include <TSAutosar.h>              /* global configuration */
#include <CanTSyn_Types.h>

/*==================[macros]=================================================*/

#if (defined CANTSYN_DEV_ERROR_DETECT) /* to prevent double declaration */
#error CANTSYN_DEV_ERROR_DETECT is already defined
#endif /* if (defined CANTSYN_DEV_ERROR_DETECT) */

/** \brief Switch for DET usage */ /* !LINKSTO CanTSyn.ASR44.ECUC_CanTSyn_00002,1 */
#define CANTSYN_DEV_ERROR_DETECT        STD_[!IF "CanTSynGeneral/CanTSynDevErrorDetect = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_VERSION_INFO_API)
#error CANTSYN_VERSION_INFO_API is already defined
#endif /* if (defined CANTSYN_VERSION_INFO_API) */

/** \brief Support of CanTSyn_GetVersionInfo()
  ** This constant defines whether the CanTSyn_GetVersionInfo is
  ** provided.
  ** STD_ON    Function is provided
  ** STD_OFF   Function is not provided
*/
/* !LINKSTO CanTSyn.ASR44.ECUC_CanTSyn_00023,1 */
#define CANTSYN_VERSION_INFO_API       STD_[!IF "CanTSynGeneral/CanTSynVersionInfoApi = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_TX_CRC_USED)
#error CANTSYN_TX_CRC_USED is already defined
#endif /* if (defined CANTSYN_TX_CRC_USED) */

/** \brief Support of Crc_CalculateCRC8H2F()
  ** This constant defines whether the Crc_CalculateCRC8H2F is
  ** used for the master feature.
  ** STD_ON    Function is used
  ** STD_OFF   Function is not used
*/
[!CALL "IsTxCRCUsed"!]
#define CANTSYN_TX_CRC_USED            STD_[!IF "$isTxCRCUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_TX_IMMEDIATE_TRANSMISSION)
#error CANTSYN_TX_IMMEDIATE_TRANSMISSION is already defined
#endif /* if (defined CANTSYN_TX_IMMEDIATE_TRANSMISSION) */

 /** \brief Support for immediate transmission of TimeSync messages.
   ** This constant defines whether Immediate Transmission is
   ** supported for the master feature or not.
   ** STD_ON    Immediate Transmission is used
   ** STD_OFF   Immediate Transmission is not used
 */
 [!CALL "IsTxImmediateTransmissionUsed"!]
#define CANTSYN_TX_IMMEDIATE_TRANSMISSION   STD_[!IF "$IsTxImmediateTransmissionUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]



#if (defined CANTSYN_RX_CRC_USED)
#error CANTSYN_RX_CRC_USED is already defined
#endif /* if (defined CANTSYN_RX_CRC_USED) */

/** \brief Support of Crc_CalculateCRC8H2F()
  ** This constant defines whether the Crc_CalculateCRC8H2F is
  ** used for the Slave feature.
  ** STD_ON    Function is used
  ** STD_OFF   Function is not used
*/
[!CALL "IsRxCRCUsed"!]
#define CANTSYN_RX_CRC_USED            STD_[!IF "$isRxCRCUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_TX_OFFSET_USED)
#error CANTSYN_TX_OFFSET_USED is already defined
#endif /* if (defined CANTSYN_TX_OFFSET_USED) */

/** \brief Support of TxOffset functionality
  ** This constant defines whether the Tx Offset functionality is
  ** available.
  ** STD_ON    Functionality available
  ** STD_OFF   Functionality is not available
*/
[!CALL "IsTxOffsetUsed"!]
#define CANTSYN_TX_OFFSET_USED    STD_[!IF "$isTxOffsetUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_RX_OFFSET_USED)
#error CANTSYN_RX_OFFSET_USED is already defined
#endif /* if (defined CANTSYN_RX_OFFSET_USED) */

/** \brief Support of RxOffset functionality
  ** This constant defines whether the Rx Offset functionality is
  ** available.
  ** STD_ON    Functionality available
  ** STD_OFF   Functionality is not available
*/
[!CALL "IsRxOffsetUsed"!]
#define CANTSYN_RX_OFFSET_USED    STD_[!IF "$isRxOffsetUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_TX_SYNC_USED)
#error CANTSYN_TX_SYNC_USED is already defined
#endif /* if (defined CANTSYN_TX_SYNC_USED) */

/** \brief Support of TxSync functionality
  ** This constant defines whether the TX Sync functionality is
  ** available.
  ** STD_ON    Functionality available
  ** STD_OFF   Functionality is not available */
[!CALL "IsTxSyncUsed"!]
#define CANTSYN_TX_SYNC_USED     STD_[!IF "$isTxSyncUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_RX_SYNC_USED)
#error CANTSYN_RX_SYNC_USED is already defined
#endif /* if (defined CANTSYN_RX_SYNC_USED) */

/** \brief Support of RxSync functionality
  ** This constant defines whether the Rx Sync functionality is
  ** available.
  ** STD_ON    Functionality available
  ** STD_OFF   Functionality is not available
*/
 [!CALL "IsRxSyncUsed"!]
 #define CANTSYN_RX_SYNC_USED     STD_[!IF "$isRxSyncUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_NUMBER_OF_SYNC_MASTERS)
#error CANTSYN_NUMBER_OF_SYNC_MASTERS is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_SYNC_MASTERS) */

/** \brief Number of the Time Master Domains with Domain Ids used for SYNC-FUP sequence */
#define CANTSYN_NUMBER_OF_SYNC_MASTERS        [!"num:i($CanTSyn_SyncTMSize)"!]U

#if (defined CANTSYN_NUMBER_OF_OFFSET_MASTERS)
#error CANTSYN_NUMBER_OF_OFFSET_MASTERS is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_OFFSET_MASTERS) */

/** \brief Number of the Time Master Domains with Domain Ids used for OFS-OFNS sequence */
#define CANTSYN_NUMBER_OF_OFFSET_MASTERS        [!"num:i($CanTSyn_OffsetTMSize)"!]U

#if (defined CANTSYN_NUMBER_OF_MASTERS)
#error CANTSYN_NUMBER_OF_MASTERS is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_MASTERS) */

/** \brief Number of the total Time Master Domains */
#define CANTSYN_NUMBER_OF_MASTERS               [!"num:i($CanTSyn_SyncTMSize + $CanTSyn_OffsetTMSize)"!]U

#if (defined CANTSYN_NUMBER_OF_SYNC_SLAVES)
#error CANTSYN_NUMBER_OF_SYNC_SLAVES is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_SYNC_SLAVES) */

/** \brief Number of the Time Slave Domains with Domain Ids used for SYNC-FUP sequence */
#define CANTSYN_NUMBER_OF_SYNC_SLAVES           [!"num:i($CanTSyn_SyncTSSize)"!]U

#if (defined CANTSYN_NUMBER_OF_OFFSET_SLAVES)
#error CANTSYN_NUMBER_OF_OFFSET_SLAVES is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_OFFSET_SLAVES) */

/** \brief Number of the Time Slave Domains with Domain Ids used for OFS-OFNS sequence */
#define CANTSYN_NUMBER_OF_OFFSET_SLAVES        [!"num:i($CanTSyn_OffsetTSSize)"!]U

#if (defined CANTSYN_NUMBER_OF_SLAVES)
#error CANTSYN_NUMBER_OF_SLAVES is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_SLAVES) */

/** \brief Number of the total Time Slave Domains */
#define CANTSYN_NUMBER_OF_SLAVES                [!"num:i($CanTSyn_SyncTSSize + $CanTSyn_OffsetTSSize)"!]U

#if (defined CANTSYN_NUMBER_OF_CONTROLLERS)
#error CANTSYN_NUMBER_OF_CONTROLLERS is already defined
#endif /* if (defined CANTSYN_NUMBER_OF_CONTROLLERS) */

/** \brief Number of referenced controllers */
#define CANTSYN_NUMBER_OF_CONTROLLERS           [!"num:i($CanTSyn_CtrlSize)"!]U


/** \brief Macros used for the CRC support for each Master Time Domain */

#if (defined CANTSYN_CRC_SUPPORTED)
#error CANTSYN_CRC_SUPPORTED is already defined
#endif /* if (defined CANTSYN_CRC_SUPPORTED) */
#define CANTSYN_CRC_SUPPORTED                 0U

#if (defined CANTSYN_CRC_NOT_SUPPORTED)
#error CANTSYN_CRC_NOT_SUPPORTED is already defined
#endif /* if (defined CANTSYN_CRC_NOT_SUPPORTED) */
#define CANTSYN_CRC_NOT_SUPPORTED             1U

 /** \brief Macros used for the CRC support for each Slave Time Domain */

#if (defined CANTSYN_CRC_VALIDATED)
#error CANTSYN_CRC_VALIDATED is already defined
#endif /* if (defined CANTSYN_CRC_VALIDATED) */
#define CANTSYN_CRC_VALIDATED                 0U

#if (defined CANTSYN_CRC_OPTIONAL)
#error CANTSYN_CRC_OPTIONAL is already defined
#endif /* if (defined CANTSYN_CRC_OPTIONAL) */
#define CANTSYN_CRC_OPTIONAL                  1U

#if (defined CANTSYN_CRC_NOT_VALIDATED)
#error CANTSYN_CRC_NOT_VALIDATED is already defined
#endif /* if (defined CANTSYN_CRC_NOT_VALIDATED) */
#define CANTSYN_CRC_NOT_VALIDATED             2U

#if (defined CANTSYN_CRC_IGNORED)
#error CANTSYN_CRC_IGNORED is already defined
#endif /* if (defined CANTSYN_CRC_IGNORED) */
#define CANTSYN_CRC_IGNORED                   3U

#if (defined CANTSYN_TX_MAX_MSG_LEN)
#error CANTSYN_TX_MAX_MSG_LEN is already defined
#endif /* if (defined CANTSYN_TX_MAX_MSG_LEN) */
[!IF "$CanTSyn_NumTxExtendedMsgFormat > 0"!]
#define CANTSYN_TX_MAX_MSG_LEN                   16U
[!ELSE!]
#define CANTSYN_TX_MAX_MSG_LEN                   8U
[!ENDIF!]

#if (defined CANTSYN_RX_MAX_MSG_LEN)
#error CANTSYN_RX_MAX_MSG_LEN is already defined
#endif /* if (defined CANTSYN_RX_MAX_MSG_LEN) */
[!IF "$CanTSyn_NumRxExtendedMsgFormat > 0"!]
#define CANTSYN_RX_MAX_MSG_LEN                   16U
[!ELSE!]
#define CANTSYN_RX_MAX_MSG_LEN                   8U
[!ENDIF!]

#if (defined CANTSYN_TX_USE_EXTENDED_MSG)
#error CANTSYN_TX_USE_EXTENDED_MSG is already defined
#endif /* if (defined CANTSYN_TX_USE_EXTENDED_MSG) */
[!IF "$CanTSyn_NumTxExtendedMsgFormat > 0"!]
#define CANTSYN_TX_USE_EXTENDED_MSG              STD_ON
[!ELSE!]
#define CANTSYN_TX_USE_EXTENDED_MSG              STD_OFF
[!ENDIF!]

#if (defined CANTSYN_RX_USE_EXTENDED_MSG)
#error CANTSYN_RX_USE_EXTENDED_MSG is already defined
#endif /* if (defined CANTSYN_RX_USE_EXTENDED_MSG) */
[!IF "$CanTSyn_NumRxExtendedMsgFormat > 0"!]
#define CANTSYN_RX_USE_EXTENDED_MSG              STD_ON
[!ELSE!]
#define CANTSYN_RX_USE_EXTENDED_MSG              STD_OFF
[!ENDIF!]

#if (defined CANTSYN_RX_FOLLOWUP_TIMEOUT_USED)
#error CANTSYN_RX_FOLLOWUP_TIMEOUT_USED is already defined
#endif /* if (defined CANTSYN_RX_FOLLOWUP_TIMEOUT_USED) */

[!CALL "IsRxFollowUpTimeoutUsed"!]
#define CANTSYN_RX_FOLLOWUP_TIMEOUT_USED     STD_[!IF "$IsRxFollowUpTimeoutUsed = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_IS_PDUINDEX_USED)
#error CANTSYN_IS_PDUINDEX_USED is already defined
#endif /* if (defined CANTSYN_IS_PDUINDEX_USED) */

#define CANTSYN_IS_PDUINDEX_USED     STD_[!IF "($isTxSyncUsed = 'true') or ($isTxOffsetUsed = 'true') or (($isRxSyncUsed = 'true') and ($IsRxFollowUpTimeoutUsed = 'true')) or (($isRxOffsetUsed = 'true') and ($IsRxFollowUpTimeoutUsed = 'true'))"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT)
#error CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT is already defined
#endif /* if (defined CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT) */

[!CALL "IsMasterTimeValidationSupported"!]
#define CANTSYN_MASTER_TIME_DOMAIN_VALIDATION_SUPPORT STD_[!IF "$isMasterTimeValidationSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]


#if (defined CANTSYN_SLAVE_TIME_DOMAIN_VALIDATION_SUPPORT)
#error CANTSYN_SLAVE_TIME_DOMAIN_VALIDATION_SUPPORT is already defined
#endif /* if (defined CANTSYN_SLAVE_TIME_DOMAIN_VALIDATION_SUPPORT) */

[!CALL "IsSlaveTimeValidationSupported"!]
#define CANTSYN_SLAVE_TIME_DOMAIN_VALIDATION_SUPPORT STD_[!IF "$isSlaveTimeValidationSupported = 'true'"!]ON[!ELSE!]OFF[!ENDIF!]

/** \brief Value of NetworkSegmentId when CanTSynEnableTimeValidation is false */
[!IF "($isMasterTimeValidationSupported = 'true') or ($isSlaveTimeValidationSupported = 'true')"!]
  #define CANTSYN_SEGMENTID_UNUSED 255U
[!ENDIF!]

/* !LINKSTO CanTSyn.ASR44.ECUC_CanTSyn_00008,1 */
/** \brief Definitions of symbolic names for all CanTSynGlobalTimeMasterConfirmationHandleIds */
[!LOOP "node:order(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[node:exists(CanTSynGlobalTimeMaster)], './CanTSynGlobalTimeDomainId')"!]
  [!VAR "prefix" = "string('CanTSynConf_CanTSynGlobalTimeMasterPdu_')"!]
  [!VAR "masterConfirmationHandleId" = "num:i(node:value(./CanTSynGlobalTimeMaster/CanTSynGlobalTimeMasterPdu/CanTSynGlobalTimeMasterConfirmationHandleId))"!]
  [!VAR "timeDomainName" = "node:name(.)"!]
  [!VAR "finalDefineMaster" = "concat($prefix, $timeDomainName, string('  '), $masterConfirmationHandleId)"!]
#define [!"$finalDefineMaster"!]U
[!ENDLOOP!]

/* !LINKSTO CanTSyn.ASR44.ECUC_CanTSyn_00013,1 */
/** \brief Definitions of symbolic names for all CanTSynGlobalTimeSlaveHandleIds */
[!LOOP "node:order(as:modconf('CanTSyn')[1]/CanTSynGlobalTimeDomain/*[node:exists(CanTSynGlobalTimeSlave)], './CanTSynGlobalTimeDomainId')"!]
  [!VAR "prefix" = "string('CanTSynConf_CanTSynGlobalTimeSlavePdu_')"!]
  [!VAR "slaveHandleId" = "num:i(node:value(./CanTSynGlobalTimeSlave/CanTSynGlobalTimeSlavePdu/CanTSynGlobalTimeSlaveHandleId))"!]
  [!VAR "timeDomainName" = "node:name(.)"!]
  [!VAR "finalDefineSlave" = "concat($prefix, $timeDomainName, string('  '), $slaveHandleId)"!]
#define [!"$finalDefineSlave"!]U
[!ENDLOOP!]


/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

#define CANTSYN_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanTSyn_MemMap.h>

[!IF "$CanTSyn_TMSize != 0"!]
/** \brief CanTSyn Master configuration
 **
 ** This is the CanTSyn Master configuration.
 ** Please note that the name of this element is configuration dependent.
 */
extern CONST(CanTSyn_TimeMasterDomainConfigType, CANTSYN_APPL_CONST) CanTSyn_TimeMasterConfig[[!"num:i($CanTSyn_TMSize)"!]];
[!ENDIF!]

[!IF "$CanTSyn_TSSize != 0"!]
/** \brief CanTSyn Slave configuration
 **
 ** This is the CanTSyn Slave configuration.
 ** Please note that the name of this element is configuration dependent.
 */
extern CONST(CanTSyn_TimeSlaveDomainConfigType, CANTSYN_APPL_CONST) CanTSyn_TimeSlaveConfig[[!"num:i($CanTSyn_TSSize)"!]];
[!ENDIF!]

#define CANTSYN_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <CanTSyn_MemMap.h>

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANTSYN_CFG_H */

/*==================[end of file]============================================*/

[!ENDCODE!]


