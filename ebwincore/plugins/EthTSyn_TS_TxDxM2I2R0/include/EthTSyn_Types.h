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

#ifndef ETHTSYN_TYPES_H
#define ETHTSYN_TYPES_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific AUTOSAR standard types */
#include <StbM.h>
#include <EthTSyn_Cfg.h> /* Module configuration */
#include <Eth_GeneralTypes.h> /* EthTrcv public types. ComStack_Types.h is included in this file */

/*==================[macros]================================================*/

#if (defined ETHTSYN_SOURCEPORTIDENTITY_SIZE)
#error ETHTSYN_SOURCEPORTIDENTITY_SIZE is already defined
#endif
/* Size of source port identity. */
#define ETHTSYN_SOURCEPORTIDENTITY_SIZE 10U

#if (defined ETHTSYN_SYNC_FRAME_SIZE)
#error ETHTSYN_SYNC_FRAME_SIZE is already defined
#endif
/* Size of a sync frame. */
#define ETHTSYN_SYNC_FRAME_SIZE 44U

#if (defined ETHTSYN_SYNCFUP_FRAME_MAX_SIZE)
#error ETHTSYN_SYNCFUP_FRAME_MAX_SIZE is already defined
#endif
/* Maximum size of a fup frame(containing all 4 supported AUTOSAR Sub-TLVs). */
#define ETHTSYN_SYNCFUP_FRAME_MAX_SIZE 131U


/*==================[type definitions]======================================*/

/** \brief Transmission mode type */
/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00033, 1 */
#if (defined ETHTSYN_TX_OFF)
#error ETHTSYN_TX_OFF is already defined
#endif
#define ETHTSYN_TX_OFF 0x00U

#if (defined ETHTSYN_TX_ON)
#error ETHTSYN_TX_ON is already defined
#endif
#define ETHTSYN_TX_ON 0x01U
typedef uint8 EthTSyn_TransmissionModeType;

/** \brief Switch port sync Tx state IDLE */
#if (defined ETHTSYN_PORT_SYN_TX_STATE_IDLE)
#error ETHTSYN_PORT_SYN_TX_STATE_IDLE is already defined
#endif
#define ETHTSYN_PORT_SYN_TX_STATE_IDLE 0x00U

/** \brief Switch port Pdelay state IDLE */
#if (defined ETHTSYN_PORT_PDELAY_STATE_IDLE)
#error ETHTSYN_PORT_PDELAY_STATE_IDLE is already defined
#endif
#define ETHTSYN_PORT_PDELAY_STATE_IDLE 0x00U

/** \brief Switch port sync Tx state WAIT_EGRESS_TS */
#if (defined ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS)
#error ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS is already defined
#endif
#define ETHTSYN_PORT_SYN_TX_STATE_WAIT_EGRESS_TS 0x02U

/** \brief Switch port sync Tx state WAIT_INGRESS_TS */
#if (defined ETHTSYN_PORT_SYN_TX_STATE_WAIT_INGRESS_TS)
#error ETHTSYN_PORT_SYN_TX_STATE_WAIT_INGRESS_TS is already defined
#endif
#define ETHTSYN_PORT_SYN_TX_STATE_WAIT_INGRESS_TS 0x03U

/** \brief Switch port sync Tx state VALID_INGRESS_TS */
#if (defined ETHTSYN_PORT_SYN_TX_STATE_VALID_INGRESS_TS)
#error ETHTSYN_PORT_SYN_TX_STATE_VALID_INGRESS_TS is already defined
#endif
#define ETHTSYN_PORT_SYN_TX_STATE_VALID_INGRESS_TS 0x04U

/** \brief Switch port sync Tx state READY_FOR_FUP */
#if (defined ETHTSYN_PORT_SYN_TX_STATE_READY_FOR_FUP)
#error ETHTSYN_PORT_SYN_TX_STATE_READY_FOR_FUP is already defined
#endif
#define ETHTSYN_PORT_SYN_TX_STATE_READY_FOR_FUP 0x05U

/** \brief Switch egress timestamp for the host port, received */
#if (defined ETHTSYN_PORT_READY_FOR_TIME_SYNC)
#error ETHTSYN_PORT_READY_FOR_TIME_SYNC is already defined
#endif
#define ETHTSYN_PORT_READY_FOR_TIME_SYNC 0x06U

/** \brief Switch port pdelay_resp wit for EGRESS_TS */
#if (defined ETHTSYN_PORT_PDELAY_WAIT_EGRESS_TS)
#error ETHTSYN_PORT_PDELAY_WAIT_EGRESS_TS is already defined
#endif
#define ETHTSYN_PORT_PDELAY_WAIT_EGRESS_TS 0x06U

/** \brief Switch port pdelay wait for INGRESS_TS */
#if (defined ETHTSYN_PORT_PDELAY_WAIT_INGRESS_TS)
#error ETHTSYN_PORT_PDELAY_WAIT_INGRESS_TS is already defined
#endif
#define ETHTSYN_PORT_PDELAY_WAIT_INGRESS_TS 0x07U

/** \brief Defines the type of EthTSyn time stamps received on each switch port. */
typedef uint8 EthTSyn_PortSyncTxStateType;

/** \brief Controller sync Tx state IDLE */
#if (defined ETHTSYN_CTRL_SYN_TX_STATE_IDLE)
#error ETHTSYN_CTRL_SYN_TX_STATE_IDLE is already defined
#endif
#define ETHTSYN_CTRL_SYN_TX_STATE_IDLE 0x0U

/** \brief Controller sync Tx state WAIT_FOR_ETH_EGRESS_TS to wait for time stamp */
#if (defined ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS)
#error ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS is already defined
#endif
#define ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_ETH_EGRESS_TS 0x02U

/** \brief Controller sync Tx state WAIT_FOR_SWT_INGRESS_TS to wait for time stamp */
#if (defined ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_SWT_INGRESS_TS)
#error ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_SWT_INGRESS_TS is already defined
#endif
#define ETHTSYN_CTRL_SYN_TX_STATE_WAIT_FOR_SWT_INGRESS_TS 0x03U

/** \brief Controller sync Tx state READY_FOR_SYN_TO_PORT */
#if (defined ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_SYN_TO_PORT)
#error ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_SYN_TO_PORT is already defined
#endif
#define ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_SYN_TO_PORT 0x04U

/** \brief Controller sync Tx state READY_FOR_FUP which means ready to send sync follow up frame */
#if (defined ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP)
#error ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP is already defined
#endif
#define ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_FUP 0x05U

/** \brief Controller sync Tx state READY_FOR_BRIDGE_SYNC which means ready to sync the bridge */
#if (defined ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_BRIDGE_SYNC)
#error ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_BRIDGE_SYNC is already defined
#endif
#define ETHTSYN_CTRL_SYN_TX_STATE_READY_FOR_BRIDGE_SYNC 0x06U

/** \brief Type for controller sync Tx state */
typedef uint8 EthTSyn_CtrlSyncTxStateType;

/** \brief Value for invalid unique ID. */
#if (defined ETHTSYN_UNIQUE_ID_INVALID)
#error ETHTSYN_UNIQUE_ID_INVALID is already defined
#endif
#define ETHTSYN_UNIQUE_ID_INVALID 0xFFFFFFFFU

/** \brief Type for frame priority */
typedef uint8 EthTSyn_CtrlDebounceFramePrioType;

#if (defined ETHTSYN_DEBOUNCE_TX_SYNC_MASK)
#error ETHTSYN_DEBOUNCE_TX_SYNC_MASK is already defined
#endif
#define ETHTSYN_DEBOUNCE_TX_SYNC_MASK             0x01U

#if (defined ETHTSYN_DEBOUNCE_TX_FUP_MASK)
#error ETHTSYN_DEBOUNCE_TX_FUP_MASK is already defined
#endif
#define ETHTSYN_DEBOUNCE_TX_FUP_MASK              0x02U

#if (defined ETHTSYN_DEBOUNCE_PDELAYREQ_MASK)
#error ETHTSYN_DEBOUNCE_PDELAYREQ_MASK is already defined
#endif
#define ETHTSYN_DEBOUNCE_PDELAYREQ_MASK           0x04U

#if (defined ETHTSYN_DEBOUNCE_PDELAYRESP_MASK)
#error ETHTSYN_DEBOUNCE_PDELAYRESP_MASK is already defined
#endif
#define ETHTSYN_DEBOUNCE_PDELAYRESP_MASK          0x08U

#if (defined ETHTSYN_DEBOUNCE_PDELAYRESP_FUP_MASK)
#error ETHTSYN_DEBOUNCE_PDELAYRESP_FUP_MASK is already defined
#endif
#define ETHTSYN_DEBOUNCE_PDELAYRESP_FUP_MASK      0x10U

#if (defined ETHTSYN_DEBOUNCE_SWT_TX_ON_HOST_MASK)
#error ETHTSYN_DEBOUNCE_SWT_TX_ON_HOST_MASK is already defined
#endif
#define ETHTSYN_DEBOUNCE_SWT_TX_ON_HOST_MASK      0x20U

/** \brief Type for bit position */

#if (defined ETHTSYN_BITPOS_TX_SYNC)
#error ETHTSYN_BITPOS_TX_SYNC is already defined
#endif
#define ETHTSYN_BITPOS_TX_SYNC                  0U

#if (defined ETHTSYN_BITPOS_TX_FUP)
#error ETHTSYN_BITPOS_TX_FUP is already defined
#endif
#define ETHTSYN_BITPOS_TX_FUP                   1U

#if (defined ETHTSYN_BITPOS_PDELAYREQ)
#error ETHTSYN_BITPOS_PDELAYREQ is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYREQ                2U

#if (defined ETHTSYN_BITPOS_PDELAYRESP)
#error ETHTSYN_BITPOS_PDELAYRESP is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYRESP               3U

#if (defined EETHTSYN_BITPOS_PDELAYRESP_FUP)
#error ETHTSYN_BITPOS_PDELAYRESP_FUP is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYRESP_FUP           4U

#if (defined ETHTSYN_BITPOS_TX_SYNC_ON_HOST)
#error ETHTSYN_BITPOS_TX_SYNC_ON_HOST is already defined
#endif
#define ETHTSYN_BITPOS_TX_SYNC_ON_HOST           5U

#if (defined ETHTSYN_BITPOS_TX_SWT)
#error ETHTSYN_BITPOS_TX_SWT is already defined
#endif
#define ETHTSYN_BITPOS_TX_SWT                   5U

/** \brief Type for pdelay calculation */
typedef uint8 EthTSyn_PdelayCalculationType;

#if (defined ETHTSYN_PDELAY_CALCULATION_IDLE)
#error ETHTSYN_PDELAY_CALCULATION_IDLE is already defined
#endif
#define ETHTSYN_PDELAY_CALCULATION_IDLE           0U

#if (defined ETHTSYN_READY_FOR_PDELAY_CALCULATION)
#error ETHTSYN_READY_FOR_PDELAY_CALCULATION is already defined
#endif
#define ETHTSYN_READY_FOR_PDELAY_CALCULATION             0xFU


#if (defined ETHTSYN_PDELAYCALC_PDELAYREQ_SENT_MASK)
#error ETHTSYN_PDELAYCALC_PDELAYREQ_SENT_MASK is already defined
#endif
#define ETHTSYN_PDELAYCALC_PDELAYREQ_SENT_MASK             0x01U

#if (defined ETHTSYN_PDELAYCALC_PDELAYRESP_RECEIVED_MASK)
#error ETHTSYN_PDELAYCALC_PDELAYRESP_RECEIVED_MASK is already defined
#endif
#define ETHTSYN_PDELAYCALC_PDELAYRESP_RECEIVED_MASK             0x03U

/** \brief Type for bit position */

#if (defined ETHTSYN_BITPOS_PDELAYREQ_SENT)
#error ETHTSYN_BITPOS_PDELAYREQ_SENT is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYREQ_SENT                   0U

#if (defined ETHTSYN_BITPOS_PDELAYREQ_EGRESSCONF)
#error ETHTSYN_BITPOS_PDELAYREQ_EGRESSCONF is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYREQ_EGRESSCONF                 1U

#if (defined ETHTSYN_BITPOS_PDELAYRESP_RECEIVED)
#error ETHTSYN_BITPOS_PDELAYRESP_RECEIVED is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYRESP_RECEIVED              2U

#if (defined ETHTSYN_BITPOS_PDELAYRESP_FUP_RECEIVED)
#error ETHTSYN_BITPOS_PDELAYRESP_FUP_RECEIVED is already defined
#endif
#define ETHTSYN_BITPOS_PDELAYRESP_FUP_RECEIVED          3U

/** \brief Defines the type of EthTSyn common master and slave shared variables. */
typedef struct
{
  Eth_TimeStampType OriginTimeStamp;
  Eth_TimeStampType EgressTimeStamp_PdelayT3;
  Eth_TimeStampType IngressTimeStamp;
  uint32 SendNextSyncFrame_Timeout;
  uint32 SendNextPdelayReqFrame_Timeout;
#if(ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
  uint32 PdelayRespPairTimeout;
#endif
  uint32 PdelayResp_BufIdx;
  uint32 Sync_BufIdx;
  uint32 PdelayReq_BufIdx;
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE))
  uint32 PdelayRespFup_BufIdx;
#endif
  uint32 DebounceTimeOffset;
#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
  uint32 RequestNonce;
  uint32 ResponseNonce;
#endif
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT != STD_ON)
  StbM_VirtualLocalTimeType RxIngressVirtualTime;
#endif
  uint16 SyncSequNumber; /* Unique ID  Master: sync frame, Slave: ingress sync frame. */
  uint16 PdelayReqSequNumber; /* Unique ID for Pdelay request frame. */
  /* To store incoming sequence number. */
  uint16 Ingress_PdelayReqSequNumber;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  uint16 ReceivedFupFrameLength;
#endif
  EthTrcv_LinkStateType LinkState;
  /* Source port identity: */
  uint8 Pdelay_Tx_SrcPortIdentity[ETHTSYN_SOURCEPORTIDENTITY_SIZE];
  uint8 Pdelay_Rx_SrcPortIdentity[ETHTSYN_SOURCEPORTIDENTITY_SIZE];
  EthTSyn_TransmissionModeType TxMode;
  uint8 Crc_Time_Flags;
  uint8 GlobalTimebaseStatus;
  uint8 LastTimeBaseUpdateCounter;
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  uint8 ReceivedSyncFrame[ETHTSYN_SYNC_FRAME_SIZE];
  uint8 ReceivedFupFrame[ETHTSYN_SYNCFUP_FRAME_MAX_SIZE];
#endif
#if(ETHTSYN_HW_TIMESTAMP_SUPPORT != STD_ON)
  Std_ReturnType GetCurrentVirtualLocalTimeRetVal;
#endif
  EthTSyn_CtrlDebounceFramePrioType Frame2Transmit;
  EthTSyn_CtrlSyncTxStateType SyncTxState; /* handles the transmission of syn and sync FUP frames */
  boolean ImmediateTransIsLocked;
#if(ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED != TS_PROD_ERR_DISABLE)
  boolean PdelayRespFupPairTimeoutInit;
#endif
#if(ETHTSYN_DEVICE_AUTHENTICATION_SUPPORT == STD_ON)
  boolean PdelayReqReceivedWithAuthChallTlv;
#endif
  StbM_UserDataType StbMUserData;
  StbM_TimeStampType StbMTimeStamp;
  StbM_VirtualLocalTimeType StbMVirtualLocalTime;
#if((ETHTSYN_TIME_RECORDING == STD_ON) && ((ETHTSYN_MAX_CTRL != ETHTSYN_MAX_SLAVE) || (ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)))
  StbM_EthTimeMasterMeasurementType SyncMasterTimeRecording;
  StbM_PdelayResponderMeasurementType PdelayResponderTimeRecording;
#endif
} EthTSyn_ControllerType;

/** \brief Defines the type of EthTSyn slave shared variables. */
typedef struct
{
  /* Variable to store the converted 16 bit value of last correction value. */
  Eth_TimeStampType LastCorrFieldHi_TS;
  Eth_TimeStampType Sync_ActualIngressTimeStamp; /* Actual ingress time stamp of sync frame. */
  Eth_TimeStampType Pdelay_TimeStamp_t1; /* To store t1 */
  Eth_TimeStampType Pdelay_TimeStamp_t2; /* To store t2 */
  Eth_TimeStampType Pdelay_TimeStamp_t3; /* To store t3 */
  Eth_TimeStampType Pdelay_TimeStamp_t4; /* To store t4 */
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  Eth_TimeIntDiffType TimeDiffHostEgressSlaveIngress; /* To store the diff T5 - T4 (figure 5.7 PRS 1.5.1) */
  /* To store the sum of the OTS and correction */
  /* Eth_TimeIntDiffType is used in order to use only valid values. For this timestamp the negative signal
     means that the the received timestamps are not ok. */
  Eth_TimeIntDiffType OriginTimeStampCorr;
#endif
  uint32 Pdelay; /* Propagation delay in nanoseconds. */
  uint32 PdelayFollowUpTimeout;
  uint32 GlobalTimeFollowUpTimeout; /* Timeout value of the Follow_Up message. */
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED != TS_PROD_ERR_DISABLE)
  uint32 SyncFupPairTimeout; /* Timeout supervision */
#endif
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  uint32 BridgeLastSyncSentTime ; /* The latest time when a sync was transmitted. */
#endif
  /* Variable to store the highest 16 bit of the last 48 bit correction value. */
  uint16 LastCorrFieldHi_ns;
#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
  /* Source port identity: */
  uint8 Sync_Rx_SrcPortIdentity[ETHTSYN_SOURCEPORTIDENTITY_SIZE];
#endif
#if (ETHTSYN_PROD_ERR_HANDLING_SYNC_FAILED == TS_PROD_ERR_REP_TO_DET)
  uint8 SyncFupDroppedPairsCt; /* Dropped Sync/Fup frames */
#endif
#if (ETHTSYN_PROD_ERR_HANDLING_PDELAY_FAILED == TS_PROD_ERR_REP_TO_DET)
  uint8 PdelayRespFupDroppedPairsCt; /* Dropped PdelayResp/RespFup frames */
#endif
  uint8 ReceivedOffsetTimeDomId; /* The offset timedomainId received in the Ofs Sub-TLV*/
  EthTSyn_PdelayCalculationType CalculatePdelay; /* Variable contains the information if a pdelay calculation can be performed or not. */
  boolean Sync_SyncFrameReceived; /* TRUE if Sync frame was received. */
  boolean OffsetTlvReceived;
  /* Time Slave shall check the Sequence Counter , unless it is the first Sync message after Initialization
     or after a Synchronization Timeout. */
  boolean CheckSeqCounter;
#if((ETHTSYN_DEV_ERROR_DETECT == STD_ON) && (ETHTSYN_MASTER_SLAVE_CONFLICT_DETECTION == STD_ON))
  /* Save the source port id of the first received sync frame. */
  boolean SaveSrcPortId;
#endif
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_ON)
  /* Boundary clock transmit sync frames . */
  boolean TriggerTxOnBridge;
#endif
  StbM_UserDataType StbMOffsetUserData;
  StbM_TimeStampType StbMOffsetTimeStamp;
#if((ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_SLAVE != 0U))
  StbM_EthTimeSlaveMeasurementType SyncSlaveTimeRecording;
  StbM_PdelayInitiatorMeasurementType PdelayInitiatorTimeRecording;
#endif /* (ETHTSYN_TIME_RECORDING == STD_ON) && (ETHTSYN_MAX_SLAVE != 0U) */
} EthTSyn_ControllerSlaveType;

#if(ETHTSYN_SWITCH_USED == STD_ON)
/** \brief Defines the type of EthTSyn time stamps received on each switch port. */
typedef struct
{
  Eth_TimeStampType SyncTimeStamp;
  Eth_TimeStampType IngPdelayFrameTimeStamp;
  Eth_TimeStampType EgressPdelayFrameTimeStamp;
#if(ETHTSYN_TIME_RECORDING == STD_ON)
#if(ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF)
  StbM_EthTimeMasterMeasurementType SyncMasterTimeRecording;
  StbM_PdelayResponderMeasurementType PdelayResponderTimeRecording;
#else
  /* If a TAB is used, all the types are needed because one slave port and at least
     one master port will be available. */
  StbM_EthTimeMasterMeasurementType SyncMasterTimeRecording;
  StbM_PdelayResponderMeasurementType PdelayResponderTimeRecording;
  StbM_EthTimeSlaveMeasurementType SyncSlaveTimeRecording;
  StbM_PdelayInitiatorMeasurementType PdelayInitiatorTimeRecording;
#endif /* ETHTSYN_BRIDGE_WITH_GTM_NOT_MGM_CPU == STD_OFF */
#endif /* ETHTSYN_TIME_RECORDING == STD_ON */
  uint32 UniqueId;
  uint32 PdelayUniqueId;
  /* To store incoming sequence number. */
  uint16 Ingress_PdelayReqSequNumber;
  /* handles the transmission of syn and sync FUP frames */
  EthTSyn_PortSyncTxStateType SyncTxState;
  /* handles the transmission of Pdelay_Resp and Pdelay_Resp FUP frames */
  EthTSyn_PortSyncTxStateType PdelayState;
  EthTSyn_CtrlDebounceFramePrioType Frame2Transmit;
  uint8 Pdelay_Rx_SrcPortIdentity[ETHTSYN_SOURCEPORTIDENTITY_SIZE];
} EthTSyn_ControllerSwtPortType;
#endif

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHTSYN_TYPES_H */
/*==================[end of file]===========================================*/
