/**
 * \file
 *
 * \brief AUTOSAR EthTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module EthTSyn.
 *
 * \version 2.2.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
[!CODE!]

#ifndef ETHTSYN_CFGTYPES_H
#define ETHTSYN_CFGTYPES_H

/*==================[includes]==============================================*/
#include <TSAutosar.h>              /* global configuration */
[!INCLUDE "EthTSyn_Precompile.m"!]
[!IF "$useDEM = 'true'"!]
#include <Dem.h>                    /* AUTOSAR Dem */
[!ENDIF!]
/*==================[macros]================================================*/

/* check for defined STD_ON macro value */
#ifndef STD_ON
#error STD_ON not defined
#endif  /* STD_ON */

/* check for defined STD_OFF macro value */
#ifndef STD_OFF
#error STD_OFF not defined
#endif  /* STD_OFF */

/*====== Configuration ====== */
#if (defined ETHTSYN_DEV_ERROR_DETECT)
#error ETHTSYN_DEV_ERROR_DETECT is already defined
#endif
/** \brief Pre-processor switch for enabling development error detection. */
#define ETHTSYN_DEV_ERROR_DETECT [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynDevErrorDetect = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION)
#error ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION is already defined
#endif
/** \brief Pre-processor switch for enabling master/slave conflict detection and notification. */
#define ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynMasterSlaveConflictDetection = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]


#if (defined ETHTSYN_DEM_REPORTING)
#error ETHTSYN_DEM_REPORTING is already defined
#endif /* if (defined ETHTSYN_DEM_REPORTING) */

#define ETHTSYN_DEM_REPORTING     [!IF "$useDEM = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Switch for DEM to DET reporting */
#define ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED   [!//
[!IF "EthTSynReportError/EthTSynSyncFailedReportError = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "EthTSynReportError/EthTSynSyncFailedReportError = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "EthTSynReportError/EthTSynSyncFailedReportError = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define ETHTSYN_E_DET_SYNC_FAILED          [!"EthTSynReportError/EthTSynSyncFailedReportErrorDetErrorId"!]U
[!ENDIF!][!//

/** \brief Switch for DEM to DET reporting */
#define ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED   [!//
[!IF "EthTSynReportError/EthTSynPdelayFailedReportError = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "EthTSynReportError/EthTSynPdelayFailedReportError = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//
 
[!IF "EthTSynReportError/EthTSynPdelayFailedReportError = 'DET'"!][!//
/** \brief Det error ID, if DEM to DET reporting is enabled */
#define ETHTSYN_E_DET_PDELAY_FAILED         [!"EthTSynReportError/EthTSynPdelayFailedReportErrorDetErrorId"!]U
[!ENDIF!][!//

/** \brief Switch for DEM to DET reporting */
#define ETHTSYN_PROD_ERR_HANDLING_UNEXPECTED_SYNC  [!//
[!IF "EthTSynReportError/EthTSynUnexpectedSyncReportError = 'DEM'"!][!//
TS_PROD_ERR_REP_TO_DEM
[!ELSEIF "EthTSynReportError/EthTSynUnexpectedSyncReportError = 'DET'"!][!//
TS_PROD_ERR_REP_TO_DET
[!ELSE!][!//
TS_PROD_ERR_DISABLE
[!ENDIF!][!//

[!IF "EthTSynReportError/EthTSynUnexpectedSyncReportError = 'DET'"!][!//
/** \brief Det error ID, if EthTSynUnexpectedSyncReportError is set to DET */
#define ETHTSYN_E_TMCONFLICT_CONFIGURED       [!"EthTSynReportError/EthTSynUnexpectedSyncReportErrorDetErrorId"!]U
[!ENDIF!][!//

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00144, 1 */
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
/** \brief Det error ID, if EthTSynMasterSlaveConflictDetection is enabled */
#define ETHTSYN_E_TMCONFLICT       0x01U
#endif

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00144, 1 */
/** \brief Det error ID, if EthTSynMasterSlaveConflictDetection is enabled */
#if (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON)
#define ETHTSYN_E_TSCONFLICT       0x02U
#endif

#if (defined ETHTSYN_VERSION_INFO_API)
#error ETHTSYN_VERSION_INFO_API is already defined
#endif
/** \brief Pre-processor switch for enabling version info API support. */
#define ETHTSYN_VERSION_INFO_API [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynVersionInfoApi = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_SEND_ANNOUNCEFRAMES)
#error ETHTSYN_SEND_ANNOUNCEFRAMES is already defined
#endif
/** \brief Pre-processor switch for enabling announce frame sending. */
#define ETHTSYN_SEND_ANNOUNCEFRAMES [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynAnnounceFrameSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO EthTSyn.ASR19-11.SWS_EthTSyn_00212, 1 */
#if (defined ETHTSYN_TIME_RECORDING)
#error ETHTSYN_TIME_RECORDING is already defined
#endif
/** \brief Pre-processor switch for enabling time recording for time validation for a specific Time Domain. */
#define ETHTSYN_TIME_RECORDING [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynTimeValidationSupport  = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_HW_TIMESTAMP_SUPPORT)
#error ETHTSYN_HW_TIMESTAMP_SUPPORT is already defined
#endif
/** \brief Pre-processor switch for enabling hardware time stamping support. */
#define ETHTSYN_HW_TIMESTAMP_SUPPORT [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynHardwareTimestampSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_MESSAGE_COMPLIANCE)
#error ETHTSYN_MESSAGE_COMPLIANCE is already defined
#endif
/** \brief Pre-processor switch which defines if IEEE TLV (for STD_ON) or AUTOSAR TLV (for STD_OFF) is used */
#define ETHTSYN_MESSAGE_COMPLIANCE [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynMessageCompliance = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT)
#error ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT is already defined
#endif
/** \brief Pre-processor switch for enabling Device Authentication support. */
#define ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynEnableDeviceAuthenticationSupport = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Support of sending only one sync frame on the host port.
           EthSwt will handle the transmission of the sync frames on the other ports.
  ** This constant defines whether the switch feature is used
  ** STD_ON    feature is used
  ** STD_OFF   feature is not used
*/
#if (defined ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT)
#error ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT is already defined
#endif
/** \brief Pre-processor switch for removing sync2 frame. */
#define ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT [!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynSendSyncFrameOnlyOnHostPort = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_BRIDGE_WITH_GTM_MGM_CPU)
#error ETHTSYN_BRIDGE_WITH_GTM_MGM_CPU is already defined
#endif /* if (defined ETHTSYN_BRIDGE_WITH_GTM_MGM_CPU) */

[!CALL "IsSwtUsed"!]
#define ETHTSYN_BRIDGE_WITH_GTM_MGM_CPU     [!IF "$IsSwtUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* !LINKSTO EthTSyn.EB.BridgeWithGTMnotMgmCPU, 1 */
/** \brief Support of SWT bridge functionality
  ** This constant defines whether the switch bridge feature is used
  ** STD_ON    feature is used
  ** STD_OFF   feature is not used
*/
#if (defined ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU)
#error ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU is already defined
#endif /* if (defined ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU) */

[!CALL "IsSwtBridgeUsed"!]
#define ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU     [!IF "$IsSwtBridgeUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/** \brief Support of SWT functionality
  ** This constant defines whether the switch feature is used
  ** STD_ON    feature is used
  ** STD_OFF   feature is not used
*/
#if (defined ETHTSYN_SWITCH_USED)
#error ETHTSYN_SWITCH_USED is already defined
#endif /* if (defined ETHTSYN_SWITCH_USED) */

#define ETHTSYN_SWITCH_USED     [!IF "$IsSwtUsed = 'true' or $IsSwtBridgeUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

/* The case when ETHTSYN_MAX_CTRL == ETHTSYN_MAX_SLAVE is the case when only Slave ports are configured in EthTSyn */
/* !LINKSTO EthTSyn.EB.NoMasterConfigured, 1 */
#if (defined ETHTSYN_MAX_CTRL)
#error ETHTSYN_MAX_CTRL is already defined
#endif
/** \brief Maximum possible number of EthTSyn controllers supported. */
#define ETHTSYN_MAX_CTRL [!"num:i($EthTSyn_MaxCtrl)"!]U

/* !LINKSTO EthTSyn.EB.NoSlaveConfigured, 1 */
#if (defined ETHTSYN_MAX_SLAVE)
#error ETHTSYN_MAX_SLAVE is already defined
#endif
[!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynVariantHandlingIsUsed = 'true'"!][!//
[!IF "node:exists(as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSyNumberOfConfiguredSlaves)"!][!//
#define ETHTSYN_MAX_SLAVE [!"node:value(as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSyNumberOfConfiguredSlaves)"!]U
[!ELSE!]
#define ETHTSYN_MAX_SLAVE 0U
[!ENDIF!]
[!ELSE!]
/** \brief Maximum possible number of EthTSyn slaves supported. */
#define ETHTSYN_MAX_SLAVE [!"num:i($EthTSyn_MaxSlave)"!]U
[!ENDIF!]


#if (defined ETHTSYN_SWITCH_PORTS_CONFIGURED)
#error ETHTSYN_SWITCH_PORTS_CONFIGURED is already defined
#endif
[!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynVariantHandlingIsUsed = 'true'"!][!//
[!IF "node:exists(as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynMaxNumberOfSwtPorts)"!][!//
/** \brief Configured switch ports */
#define ETHTSYN_SWITCH_PORTS_CONFIGURED [!"node:value(as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynMaxNumberOfSwtPorts)"!]U
[!ELSE!]
#define ETHTSYN_SWITCH_PORTS_CONFIGURED 0U
[!ENDIF!]
[!ELSE!]
/** \brief Configured switch ports */
#define ETHTSYN_SWITCH_PORTS_CONFIGURED [!CALL "EthTSyn_MaxSwtPorts"!]U
[!ENDIF!]

/** \brief Macros used for the CRC support for each Master Time Domain */
#if (defined ETHTSYN_TX_CRC_USED)
#error ETHTSYN_TX_CRC_USED is already defined
#endif /* if (defined ETHTSYN_TX_CRC_USED) */

/** \brief Support of Crc_CalculateCRC8H2F()
  ** This constant defines whether the Crc_CalculateCRC8H2F is
  ** used for the master feature.
  ** STD_ON    Function is used
  ** STD_OFF   Function is not used
*/
[!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynVariantHandlingIsUsed = 'true'"!][!//
#define ETHTSYN_TX_CRC_USED STD_ON
[!ELSE!]
[!CALL "IsTxCRCUsed"!]
#define ETHTSYN_TX_CRC_USED     [!IF "$isTxCRCUsed = 'true' or $IsSwtBridgeUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!]

#if (defined ETHTSYN_RX_CRC_USED)
#error ETHTSYN_RX_CRC_USED is already defined
#endif /* if (defined ETHTSYN_RX_CRC_USED) */

/** \brief Support of Crc_CalculateCRC8H2F()
  ** This constant defines whether the Crc_CalculateCRC8H2F is
  ** used for the Slave feature.
  ** STD_ON    Function is used
  ** STD_OFF   Function is not used
*/
[!IF "as:modconf('EthTSyn')[1]/EthTSynGeneral/EthTSynVariantHandlingIsUsed = 'true'"!][!//
#define ETHTSYN_RX_CRC_USED STD_ON
[!ELSE!]
[!CALL "IsRxCRCUsed"!]
#define ETHTSYN_RX_CRC_USED     [!IF "$isRxCRCUsed = 'true' or $IsSwtBridgeUsed = 'true'"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]
[!ENDIF!]

/** \brief Support of SWT functionality
  ** This constant defines whether the switch feature is used
  ** STD_ON    feature is used
  ** STD_OFF   feature is not used
*/

#if (defined ETHTSYN_CRC_SUPPORTED)
#error ETHTSYN_CRC_SUPPORTED is already defined
#endif /* if (defined ETHTSYN_CRC_SUPPORTED) */
#define ETHTSYN_CRC_SUPPORTED                 0U

#if (defined ETHTSYN_CRC_NOT_SUPPORTED)
#error ETHTSYN_CRC_NOT_SUPPORTED is already defined
#endif /* if (defined ETHTSYN_CRC_NOT_SUPPORTED) */
#define ETHTSYN_CRC_NOT_SUPPORTED             1U

 /** \brief Macros used for the CRC support for each Slave Time Domain */
#if (defined ETHTSYN_CRC_VALIDATED)
#error ETHTSYN_CRC_VALIDATED is already defined
#endif /* if (defined ETHTSYN_CRC_VALIDATED) */
#define ETHTSYN_CRC_VALIDATED                 0U

#if (defined ETHTSYN_CRC_NOT_VALIDATED)
#error ETHTSYN_CRC_NOT_VALIDATED is already defined
#endif /* if (defined ETHTSYN_CRC_NOT_VALIDATED) */
#define ETHTSYN_CRC_NOT_VALIDATED             1U

#if (defined ETHTSYN_CRC_IGNORED)
#error ETHTSYN_CRC_IGNORED is already defined
#endif /* if (defined ETHTSYN_CRC_IGNORED) */
#define ETHTSYN_CRC_IGNORED                   2U

#if (defined ETHTSYN_CRC_OPTIONAL)
#error ETHTSYN_CRC_OPTIONAL is already defined
#endif /* if (defined ETHTSYN_CRC_OPTIONAL) */
#define ETHTSYN_CRC_OPTIONAL                   3U

/*==================[type definitions]======================================*/

/** \brief Type for internal port index */
typedef uint8 EthTSyn_IntPortIdxType;

/** \brief Defines the type of the EthTSyn controller configuration structure. */
typedef struct
{
  uint32 GlobalTimeDebounceTime;
  uint32 CyclicMsgResumeTime;
  /* Number of main function cycles until next sync frame sent. */
  uint32 SyncTxInterval;
  /* Number of main function cycles until next Pdelay request frame sent. */
  uint32 PdelayReqTxInterval;
  /* Timeout value for Pdelay_Resp and Pdelay_Resp_Follow_Up after a Pdelay_Req
     has been transmitted resp. a Pdelay_Resp has been received.
     A value of 0 deactivates this timeout observation.*/
  uint32 PdelayRespAndRespFollowUpTimeout;
  uint32 DefaultPdelayNs;
  /* Timeout value of the Follow_Up message (of the subsequent Sync message). */
  uint32 GlobalTimeFollowUpTimeout;
#if(ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  uint32 ReceiveSyncFupPairTimeout;
#endif
#if(ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
  uint32 PdelayRespFupPairsPeriod;
#endif
  uint32 PdelayLatencyThreshold; /* EthTSynPdelayLatencyThreshold */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  uint32 BridgeTxPeriod; /* EthTSynBridgeTxPeriod */
#endif
  uint16 SequenceCounterJumpWidth; /* EthTSynGlobalTimeSequenceCounterJumpWidth */
  boolean TimeSyncMaster; /* TRUE if master, FALSE if slave. */
  uint8 EthIfCtrlIdx;
  uint8 SyncGlobalTimeDomainId; /* The ID of the Sync TimeDomain */
  uint8 OfsGlobalTimeDomainId;  /* The ID of the Offset TimeDomain */
  uint8 OffsetTimeBaseId;       /* The ID of the StbM Offset timebase */
  uint8 TimeBaseId;
  boolean VltStbMIsEthFreerunningCounter;
  /* Filtering (Slave: Pdelay): Number of shifts performed on the delta between old and new
     value. */
  uint8 SyncTxMsgLogInterval; /* Sync Tx period in 2^x format. */
  uint8 PdelayTxMsgLogInterval; /* Pdelay Tx period in 2^x format. */
  uint8 FilterDeltaShift; /* The value of the Pdelay filter */
  uint8 FramePriority; /* Ethernet frame priority for VLAN */
  uint8 IsTxCrcSecuredUsed; /* Configuration parameter that activates/deactivates the CRC support */
  uint8 RxCrcValidated; /* Configuration parameter that activates/deactivates the CRC support */
  uint8 DataIdList[16U];
  EthTSyn_IntPortIdxType PortStart;
  EthTSyn_IntPortIdxType PortCount;
  EthTSyn_IntPortIdxType PortMgt;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  EthTSyn_IntPortIdxType PortSlave;
  boolean SimpleBridge;
  boolean SynchronizeSimpleBridge;
  boolean SimpleBridgeValidateSync;
  boolean SimpleBridgeTSynSendSync;
#endif
  boolean PdelayRespEnable;
  boolean ImmediateTimeSync;
  boolean TLVFollowUpTimeSubTLV; /* Enable or disable AUTOSAR Follow_Up TLV Time Sub-TLV */
  boolean TLVFollowUpStatusSubTLV; /* Enable or disable AUTOSAR Follow_Up TLV Status Sub-TLV */
  boolean TLVFollowUpUserDataSubTLV; /* Enable or disable AUTOSAR Follow_Up TLV User Data Sub-TLV */
  boolean TLVFollowUpOFSSubTLV ; /* Enable or disable AUTOSAR Follow_Up TLV Ofs Sub-TLV */
  boolean CrcMessageLength;
  boolean CrcDomainNumber;
  boolean CrcCorrectionField;
  boolean CrcSourcePortIdentity;
  boolean CrcSequenceId;
  boolean CrcPreciseOriginTimestamp;
  boolean SwtPortIdxInCorrField;
#if(ETHTSYN_TIME_RECORDING == STD_ON)
  /* Time recording for time validation for a specific Time Base */
  boolean TimeValidationEnabled;
#endif
#if(ETHTSYN_DEM_REPORTING == STD_ON)
  Dem_EventIdType EventIdSyncFailed;      /**< Event Id to be passed to Dem */
  Dem_EventIdType EventIdPdelayFailed;    /**< Event Id to be passed to Dem */
  Dem_EventIdType EventIdUnexpectedSync;  /**< Event Id to be passed to Dem */
#endif
} EthTSyn_ControllerConfigType;

#if(ETHTSYN_SWITCH_USED == STD_ON)
/** \brief  */
typedef struct
{
  uint16 EthSwtIdx;
  uint16 EthSwtPortIdx;
#if(ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF)
  uint16 EthTSynSourcePortId;
#endif /* ETHTSYN_SEND_SYNC_ONLY_ON_HOST_PORT == STD_OFF */
  boolean PdelayRespEnable;
#if(ETHTSYN_TIME_RECORDING == STD_ON)
  /* Time recording for time validation for a specific Port */
  boolean TimeValidationEnabled;
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
} EthTSyn_SwitchConfigType;
#endif /* ETHTSYN_SWITCH_USED == STD_ON */

/** \brief Defines the type of EthTSyn configuration structure. */
/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00032, 1 */
typedef struct
{
  CONST(uint8, ETHTSYN_APPL_CONST) MaxEthTSynCtrlIdx;
  CONST(uint8, ETHTSYN_APPL_CONST) MaxEthTSynSlaveIdx;
  /* EthTSyn controller configuration. */
  CONST(EthTSyn_ControllerConfigType, ETHTSYN_APPL_CONST) EthTSynCtrl[[!"num:i($EthTSyn_MaxCtrl)"!]U];
#if((ETHTSYN_SWITCH_USED == STD_ON))
  /* Pointer to the relevant information of the configured switch */
  CONSTP2CONST(EthTSyn_SwitchConfigType, AUTOMATIC, ETHTSYN_APPL_CONST) SwitchConfig;
  uint8 NrOfSwitchPortsConfigured;
#endif
} EthTSyn_ConfigType;

/** \brief Defensive programming */
#if(ETHTSYN_DEV_ERROR_DETECT == STD_ON)



/*------------------[Defensive programming]---------------------------------*/
[!SELECT "EthTSynDefensiveProgramming"!][!//

#if (defined ETHTSYN_DEFENSIVE_PROGRAMMING_ENABLED)
#error ETHTSYN_DEFENSIVE_PROGRAMMING_ENABLED is already defined
#endif
/** \brief Defensive programming usage
 **
 ** En- or disables the usage of the defensive programming */
#define ETHTSYN_DEFENSIVE_PROGRAMMING_ENABLED   [!//
[!IF "(../EthTSynGeneral/EthTSynDevErrorDetect  = 'true') and (EthTSynDefProgEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_PRECONDITION_ASSERT_ENABLED)
#error ETHTSYN_PRECONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Precondition assertion usage
 **
 ** En- or disables the usage of precondition assertion checks */
#define ETHTSYN_PRECONDITION_ASSERT_ENABLED     [!//
[!IF "(../EthTSynGeneral/EthTSynDevErrorDetect  = 'true') and (EthTSynDefProgEnabled = 'true') and (EthTSynPrecondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_POSTCONDITION_ASSERT_ENABLED)
#error ETHTSYN_POSTCONDITION_ASSERT_ENABLED is already defined
#endif
/** \brief Postcondition assertion usage
 **
 ** En- or disables the usage of postcondition assertion checks */
#define ETHTSYN_POSTCONDITION_ASSERT_ENABLED    [!//
[!IF "(../EthTSynGeneral/EthTSynDevErrorDetect  = 'true') and (EthTSynDefProgEnabled = 'true') and (EthTSynPostcondAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_UNREACHABLE_CODE_ASSERT_ENABLED)
#error ETHTSYN_UNREACHABLE_CODE_ASSERT_ENABLED is already defined
#endif
/** \brief Unreachable code assertion usage
 **
 ** En- or disables the usage of unreachable code assertion checks */
#define ETHTSYN_UNREACHABLE_CODE_ASSERT_ENABLED [!//
[!IF "(../EthTSynGeneral/EthTSynDevErrorDetect  = 'true') and (EthTSynDefProgEnabled = 'true') and (EthTSynUnreachAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_INVARIANT_ASSERT_ENABLED)
#error ETHTSYN_INVARIANT_ASSERT_ENABLED is already defined
#endif
/** \brief Invariant assertion usage
 **
 ** En- or disables the usage of invariant assertion checks */
#define ETHTSYN_INVARIANT_ASSERT_ENABLED        [!//
[!IF "(../EthTSynGeneral/EthTSynDevErrorDetect  = 'true') and (EthTSynDefProgEnabled = 'true') and (EthTSynInvariantAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

#if (defined ETHTSYN_STATIC_ASSERT_ENABLED)
#error ETHTSYN_STATIC_ASSERT_ENABLED is already defined
#endif
/** \brief Static assertion usage
 **
 ** En- or disables the usage of static assertion checks */
#define ETHTSYN_STATIC_ASSERT_ENABLED           [!//
[!IF "(../EthTSynGeneral/EthTSynDevErrorDetect  = 'true') and (EthTSynDefProgEnabled = 'true') and (EthTSynStaticAssertEnabled = 'true')"!]STD_ON[!ELSE!]STD_OFF[!ENDIF!]

[!ENDSELECT!][!//






/*------------------------[Defensive programming]----------------------------*/

#if (defined ETHTSYN_PRECONDITION_ASSERT)
#error ETHTSYN_PRECONDITION_ASSERT is already defined
#endif

#if (defined ETHTSYN_PRECONDITION_ASSERT_NO_EVAL)
#error ETHTSYN_PRECONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETHTSYN_PRECONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHTSYN_PRECONDITION_ASSERT(Condition, ApiId) \
  DET_PRECONDITION_ASSERT((Condition), ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHTSYN_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_PRECONDITION_ASSERT_NO_EVAL((Condition), ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))
#else
#define ETHTSYN_PRECONDITION_ASSERT(Condition, ApiId)
#define ETHTSYN_PRECONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHTSYN_POSTCONDITION_ASSERT)
#error ETHTSYN_POSTCONDITION_ASSERT is already defined
#endif

#if (defined ETHTSYN_POSTCONDITION_ASSERT_NO_EVAL)
#error ETHTSYN_POSTCONDITION_ASSERT_NO_EVAL is already defined
#endif

#if (ETHTSYN_POSTCONDITION_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHTSYN_POSTCONDITION_ASSERT(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT((Condition), ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHTSYN_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_POSTCONDITION_ASSERT_NO_EVAL((Condition), ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))
#else
#define ETHTSYN_POSTCONDITION_ASSERT(Condition, ApiId)
#define ETHTSYN_POSTCONDITION_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHTSYN_INVARIANT_ASSERT)
#error ETHTSYN_INVARIANT_ASSERT is already defined
#endif

#if (defined ETHTSYN_INVARIANT_ASSERT_NO_EVAL)
#error ETHTSYN_INVARIANT_ASSERT_NO_EVAL is already defined
#endif

#if (ETHTSYN_INVARIANT_ASSERT_ENABLED == STD_ON)
/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHTSYN_INVARIANT_ASSERT(Condition, ApiId) \
  DET_INVARIANT_ASSERT((Condition), ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))

/** \brief Report an assertion violation to Det
 **

 ** \param[in] Condition The condition which is verified
 ** \param[in] ApiId The service ID of the API function */
#define ETHTSYN_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId) \
  DET_INVARIANT_ASSERT_NO_EVAL((Condition), ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))
#else
#define ETHTSYN_INVARIANT_ASSERT(Condition, ApiId)
#define ETHTSYN_INVARIANT_ASSERT_NO_EVAL(Condition, ApiId)
#endif

#if (defined ETHTSYN_STATIC_ASSERT)
# error ETHTSYN_STATIC_ASSERT is already defined
#endif
#if (ETHTSYN_STATIC_ASSERT_ENABLED == STD_ON)
/** \brief Report an static assertion violation to Det
 **
 ** \param[in] Condition Condition which is violated */
# define ETHTSYN_STATIC_ASSERT(expr) DET_STATIC_ASSERT(expr)
#else
# define ETHTSYN_STATIC_ASSERT(expr)
#endif

#if (defined ETHTSYN_UNREACHABLE_CODE_ASSERT)
#error ETHTSYN_UNREACHABLE_CODE_ASSERT is already defined
#endif
#if (ETHTSYN_UNREACHABLE_CODE_ASSERT_ENABLED == STD_ON)
/** \brief Report an unreachable code assertion violation to Det
 **

 ** \param[in] ApiId Service ID of the API function */
#define ETHTSYN_UNREACHABLE_CODE_ASSERT(ApiId) \
  DET_UNREACHABLE_CODE_ASSERT(ETHTSYN_MODULE_ID, ETHTSYN_INSTANCE_ID, (ApiId))
#else
#define ETHTSYN_UNREACHABLE_CODE_ASSERT(ApiId)
#endif

#if (defined ETHTSYN_INTERNAL_API_ID)
#error ETHTSYN_INTERNAL_API_ID is already defined
#endif
/** \brief API ID of module internal functions to be used in assertions */
#define ETHTSYN_INTERNAL_API_ID DET_INTERNAL_API_ID


#endif /* ETHIF_DEV_ERROR_DETECT == STD_ON */

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHTSYN_CFGTYPES_H */
/*==================[end of file]===========================================*/
[!ENDCODE!]
