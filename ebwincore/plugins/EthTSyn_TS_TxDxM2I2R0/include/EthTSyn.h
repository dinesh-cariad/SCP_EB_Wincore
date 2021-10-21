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

#ifndef ETHTSYN_H
#define ETHTSYN_H

/*==================[inclusions]============================================*/

#include <TSAutosar.h> /* Specific standard types */
#include <EthTSyn_Cfg.h> /* Generated configuration */
#include <EthTSyn_Types.h> /* Module public types */
#include <StbM.h> /* StbM public types */

/*==================[macros]================================================*/

#if (defined ETHTSYN_INSTANCE_ID)
#error ETHTSYN_INSTANCE_ID is already defined
#endif
/** \brief Module instance ID
 *
 * Defines the instance number of this module. Since multiple instances are not supported this ID
 * is always zero.
 **/
#define ETHTSYN_INSTANCE_ID 0U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00035.ServiceId, 1 */
#if (defined ETHTSYN_SID_INIT)
#error ETHTSYN_SID_INIT is already defined
#endif
/** \brief Defines API id of function EthTSyn_Init(). */
#define ETHTSYN_SID_INIT 0x01U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00036.ServiceId, 1 */
#if (defined ETHTSYN_SID_GETVERSIONINFO)
#error ETHTSYN_SID_GETVERSIONINFO is already defined
#endif
/** \brief Defines API id of function EthTSyn_GetVersionInfo(). */
#define ETHTSYN_SID_GETVERSIONINFO 0x02U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00039.ServiceId, 1 */
#if (defined ETHTSYN_SID_SETTRANSMISSIONMODE)
#error ETHTSYN_SID_SETTRANSMISSIONMODE is already defined
#endif
/** \brief Defines API id of function EthTSyn_SetTransmissionMode(). */
#define ETHTSYN_SID_SETTRANSMISSIONMODE 0x05U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00044.ServiceId, 1 */
#if (defined ETHTSYN_SID_MAINFUNCTION)
#error ETHTSYN_SID_MAINFUNCTION is already defined
#endif
/** \brief Defines API id of function EthTSyn_MainFunction(). */
#define ETHTSYN_SID_MAINFUNCTION 0x09U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00040.ServiceId, 1 */
#if (defined ETHTSYN_SID_RXINDICATION)
#error ETHTSYN_SID_RXINDICATION is already defined
#endif
/** \brief Defines API id of function EthTSyn_RxIndication(). */
#define ETHTSYN_SID_RXINDICATION 0x06U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00042.ServiceId, 1 */
#if (defined ETHTSYN_SID_TXCONFIRMATION)
#error ETHTSYN_SID_TXCONFIRMATION is already defined
#endif
/** \brief Defines API id of function EthTSyn_TxConfirmation(). */
#define ETHTSYN_SID_TXCONFIRMATION 0x07U

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00043.ServiceId, 1 */
#if (defined ETHTSYN_SID_TRCVLINKSTATECHG)
#error ETHTSYN_SID_TRCVLINKSTATECHG is already defined
#endif
/** \brief Defines API id of function EthTSyn_TrcvLinkStateChg(). */
#define ETHTSYN_SID_TRCVLINKSTATECHG 0x08U

#if (defined ETHTSYN_SID_ETHSWTPTPTIMESTAMPINDICATION)
#error ETHTSYN_SID_ETHSWTPTPTIMESTAMPINDICATION is already defined
#endif
/** \brief Defines API id of function EthTSyn_EthSwtPtpTimeStampIndication(). */
#define ETHTSYN_SID_ETHSWTPTPTIMESTAMPINDICATION 0xE0U

#if (defined ETHTSYN_SID_ETHSWTPTPRXINDICATION)
#error ETHTSYN_SID_ETHSWTPTPRXINDICATION is already defined
#endif
/** \brief Defines API id of function EthTSyn_EthSwtPtpRxIndication(). */
#define ETHTSYN_SID_ETHSWTPTPRXINDICATION 0xE1U

/* === Module Det error codes === */
/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00030_ValueCheck_UNINIT, 1 */
#if (defined ETHTSYN_E_UNINIT)
#error ETHTSYN_E_UNINIT is already defined
#endif
#define ETHTSYN_E_UNINIT 0x20U /**< DET error code: Module not initialized */

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00030_ValueCheck_INIT_FAILED, 1 */
#if (defined ETHTSYN_E_INIT_FAILED)
#error ETHTSYN_E_INIT_FAILED is already defined
#endif
#define ETHTSYN_E_INIT_FAILED 0x21U /**< DET error code: EthTSyn initialization failed */

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00030_ValueCheck_CTRL_IDX, 1 */
#if (defined ETHTSYN_E_CTRL_IDX)
#error ETHTSYN_E_CTRL_IDX is already defined
#endif
#define ETHTSYN_E_CTRL_IDX 0x22U /**< DET error code: Invalid controller index */


/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00030_ValueCheck_PARAM_POINTER, 1 */
#if (defined ETHTSYN_E_PARAM_POINTER)
#error ETHTSYN_E_PARAM_POINTER is already defined
#endif
#define ETHTSYN_E_PARAM_POINTER 0x23U /**< DET error code: Invalid pointer (NULL_PTR) */

/* !LINKSTO EthTSyn.ASR44.SWS_EthTSyn_00030_ValueCheck_PARAM, 1 */
#if (defined ETHTSYN_E_PARAM)
#error ETHTSYN_E_PARAM is already defined
#endif
#define ETHTSYN_E_PARAM 0x24U /**< DET error code: Invalid parameter */

#if (defined ETHTSYN_INTERNAL_SVCID)
#error ETHTSYN_INTERNAL_SVCID is already defined
#endif
/** \brief Defines API id of internal functions
 **/
#define ETHTSYN_INTERNAL_SVCID                 0xFFU
/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/** \brief The data pointer used to get rid of MISRA violation */
typedef P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) EthTSyn_PtrUint8;

/*==================[external function declarations]========================*/
#define ETHTSYN_START_SEC_CODE
#include <EthTSyn_MemMap.h>

#if(ETHTSYN_VERSION_INFO_API == STD_ON)
/**
 * \brief API to get the module version information.
 *
 * This service returns the version information of this module.
 *
 * \param[out] VersionInfo - Pointer to return the module version information.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Reentrant}
 * \Synchronicity{Synchronous}
 */

FUNC(void, ETHTSYN_CODE) EthTSyn_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) VersionInfo
);
#endif /* ETHTSYN_VERSION_INFO_API */
/**
 * \brief Initializes the EthTSyn module.
 *
 * This service initializes the EthTSyn module. It shall be the first function of the module to be
 * called.
 *
 * \param[in] CfgPtr - Address of the configuration data structure.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_Init
(
  P2CONST(EthTSyn_ConfigType, AUTOMATIC, ETHTSYN_APPL_CONST) CfgPtr
);

/**
 * \brief This service enables/disables the transmission of frames for the whole module.
 *
 * \param[in] CtrlIdx - Index of the EthTSyn controller.
 * \param[in] Mode - Mode to indicate if frame shall be transmitted.
 *
 * \return void
 *
 * \ServiceID{0x05}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_SetTransmissionMode
(
  uint8 CtrlIdx,
  EthTSyn_TransmissionModeType Mode
);

/**
 * \brief EthTSyn module main function.
 *
 * \ProductionError{ETHTSYN_E_SYNC_FAILED, thrown\, if more than 12 pairs of Sync-/Follow_Up messages
 *                  in a row are dropped.}
 * \ProductionError{ETHTSYN_E_PDELAY_FAILED, thrown\, if more than 6 pairs of Pdelay_Resp/
 *                  Pdelay_Resp_Follow_Up messages in a row are dropped.}
 *
 * \ServiceID{0x09}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_MainFunction(void);


/**
 * \brief Rx-Indication function
 *
 * This service is called by EthIf in case a reception is indicated.
 *
 * \param[in] CtrlIdx - Index of the controller within the context of the EthIf.
 * \param[in] FrameType - Ethernet Frame type.
 * \param[in] IsBroadcast - Indicates whether Ethernet destination address is a broadcast address
 *                          or not.
 * \param[in] PhysAddrPtr - Pointer  to  Physical  source  address  (MAC  address  in  network  byte
 *                          order) of received Ethernet frame.
 * \param[in] DataPtr - Address of the received payload.
 * \param[in] LenByte - Length of the payload contained in the received Rx buffer.
 *
 * \ProductionError{ETHTSYN_E_SYNC_FAILED, thrown\, if more than 12 pairs of Sync-/Follow_Up messages
 *                  in a row are dropped.}
 * \ProductionError{ETHTSYN_E_PDELAY_FAILED, thrown\, if more than 6 pairs of Pdelay_Resp/
 *                  Pdelay_Resp_Follow_Up messages in a row are dropped.}
 * \ProductionError{ETHTSYN_E_UNEXPECTED_SYNC, thrown\, if a SYNC frame is received on an Ethernet port configured as master.}
 *
 * \ServiceID{0x06}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_RxIndication
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint16 LenByte
);

/**
 * \brief Tx-Confirmation callback function.
 *
 * This service is called by EthIf in case a transmission is confirmed.
 *
 * \param[in] CtrlIdx - Index of the controller within the context of the EthIf.
 * \param[in] BufIdx - Index of the transmitted buffer.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_TxConfirmation
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

/**
 * \brief Transceiver link state change indication.
 *
 * This service indicates a change of the transceiver link change.
 *
 * \param[in] CtrlIdx - Index of the EthIf controller associated with the EthTrcv.
 * \param[in] LinkState - New state the transceiver link has changed to.
 *
 * \ServiceID{0x08}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_TrcvLinkStateChg
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType LinkState
);

#if(ETHTSYN_SWITCH_USED == STD_ON)
/**
 * \brief EthTSyn TimeStampIndication function.
 *
 */
extern FUNC(void, ETHTSYN_CODE) EthTSyn_EthSwtPtpTimeStampIndication
(
  uint8 EthIfCtrlIdx,
  EthSwt_MgmtInfoType MgmtInfo,
  uint32 UniqueId,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) PortTimeStampPtr
);

/**
 * \brief EthTSyn SwtRxIndication function.
 *
 */
extern FUNC(boolean, ETHTSYN_CODE) EthTSyn_EthSwtPtpRxIndication
(
  P2CONST(EthSwt_MgmtInfoType, AUTOMATIC, ETHTSYN_APPL_DATA) MgmtInfoPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETHTSYN_APPL_DATA) IngressTimeStampPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHTSYN_APPL_DATA) DataPtr,
  uint8 EthIfCtrlIdx,
  uint16 Len,
  uint32 UniqueId
);
#endif

/*==================[internal function declarations]========================*/

#define ETHTSYN_STOP_SEC_CODE
#include <EthTSyn_MemMap.h>
/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

#endif /* ifndef ETHTSYN_H */
/*==================[end of file]===========================================*/
