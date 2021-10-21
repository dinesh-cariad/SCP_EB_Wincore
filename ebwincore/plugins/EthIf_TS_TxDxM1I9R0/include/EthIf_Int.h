/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ETHIF_INT_H
#define ETHIF_INT_H


/*==================[inclusions]============================================*/
#include <TSAutosar.h> /* EB specific standard types */
#include <ComStack_Types.h>
#include <Eth_GeneralTypes.h>   /* Ethernet general public types */
#include <EthIf_Cfg.h>          /* Module configurable parameters */
#include <EthIf_Types.h>
#include <EthIf_Types_ExtGen.h> /* Module XGEN generated EthIf_ConfigType types file */
#include <EthIf_DrvTypes.h>

/*==================[macros]================================================*/


/*==================[type definitions]======================================*/

typedef uint8 EthIf_RefCntType;

typedef uint16 EthIf_TimerType;

/** \brief Defines EthIf objects mode state
 **/
#if (defined ETHIF_MODESTATE_SET)
#error ETHIF_MODESTATE_SET is already defined
#endif
#define ETHIF_MODESTATE_SET 0U

#if (defined ETHIF_MODESTATE_REQUESTED)
#error ETHIF_MODESTATE_REQUESTED is already defined
#endif
#define ETHIF_MODESTATE_REQUESTED 1U

typedef uint8 EthIf_ModeStateType;

typedef struct
{
  EthIf_TimerType SetRepeatTimer;
  EthIf_RefCntType RefCnt;   /* Active Ethernet controller reference counter */
  boolean IsInitialized;
  Eth_ModeType Mode;
  EthIf_ModeStateType ModeState;
} EthIf_PhyCtrlStateType;

typedef struct
{
  EthIf_TimerType SetRepeatTimer;
  Eth_ModeType Mode;
  EthIf_ModeStateType ModeState;
  EthTrcv_LinkStateType TrcvLinkState;
#if (ETHIF_DEVICEAUTHENTICATION_API == STD_ON)
  boolean Disabled;
#endif /* ETHIF_DEVICEAUTHENTICATION_API == STD_ON */
} EthIf_CtrlStateType;

typedef struct
{
  EthIf_TimerType SetRepeatTimer;
  boolean IsInitialized;
  Eth_ModeType Mode;
  EthIf_ModeStateType ModeState;
  EthTrcv_LinkStateType TrcvLinkState;
#if (ETHIF_WAKEUPSUPPORT_API == STD_ON)
  EthTrcv_WakeupModeType TrcvWakeupMode;
#endif
} EthIf_TrcvStateType;

typedef struct
{
  EthIf_TimerType SetRepeatTimer;
  EthIf_TimerType SwitchOffTimer;
  EthIf_RefCntType RefCnt;
  uint8 CycleCnt;
  Eth_ModeType Mode;
  EthIf_ModeStateType ModeState;
  EthTrcv_LinkStateType TrcvLinkState;
} EthIf_PortStateType;

typedef struct
{
  EthIf_TimerType SwitchOffTimer;
  Eth_ModeType Mode;
  EthIf_ModeStateType ModeState;
  EthTrcv_LinkStateType TrcvLinkState;
  uint8 CycleCnt;
} EthIf_PortGroupStateType;

/** \brief Data Type that represents the Ethernet interface switch port index.
 * The index is zero based and unique for every configured switch port group. */
typedef uint8 EthIf_SwtPortIdxType;

/** \brief Function pointer type for user defined Up_RxIndication() */
typedef P2FUNC(void, TYPEDEF, EthIf_Up_RxIndication_FpType)
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
);

/** \brief Function pointer type for user defined Up_TxConfirmation() */
typedef P2FUNC(void, TYPEDEF, EthIf_Up_TxConfirmation_FpType)
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

/** \brief Function pointer type for Up_TrcvLinkStateChg() */
typedef P2FUNC(void, TYPEDEF, EthIf_Up_TrcvLinkStateChg_FpType)
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType TransceiverLinkState
);


/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/
#define ETHIF_START_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>

/** \brief UL_RxIndication() function pointer list
 *
 * This array contains user defined UL_RxIndication function pointers. The list is sorted
 * based on frame type owner.
 */
extern CONST(EthIf_Up_RxIndication_FpType, ETHIF_APPL_CONST) EthIf_Up_RxIndication_FpList[];

/** \brief UL_TxConfirmation() function pointer list
 *
 * This array contains user defined UL_TxConfirmation() function pointers.
 */
extern CONST(EthIf_Up_TxConfirmation_FpType, ETHIF_APPL_CONST) EthIf_Up_TxConfirmation_FpList[];

/** \brief UL_TrcvLinkStateChg() function pointer list
 *
 * This array contains user defined UL_TrcvLinkStateChg() function pointers.
 */
extern CONST(EthIf_Up_TrcvLinkStateChg_FpType, ETHIF_APPL_CONST) EthIf_Up_TrcvLinkStateChg_FpList[];

#if(ETHIF_ETH_BSWIMPL > 1)
/** \brief Configuration of Eth driver function pointers
 **
 ** This array contains the function pointers for all the interfaces
 ** of the Eth controller.
 */
extern CONST(EthIf_EthFuncConfigType, ETHIF_APPL_CONST) EthIf_EthFuncConfig[];
#endif
#if(ETHIF_ETHTRCV_BSWIMPL > 1)
/** \brief Configuration of Eth transceiver function pointers
 **
 ** This array contains the function pointers for all the interfaces
 ** of the Eth transceiver.
 */
extern CONST(EthIf_EthTrcvFuncConfigType, ETHIF_APPL_CONST) EthIf_EthTrcvFuncConfig[];
#endif
#define ETHIF_STOP_SEC_CONFIG_DATA_UNSPECIFIED
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_CONFIG_DATA_8
#include <EthIf_MemMap.h>

/** \brief Maximum number of configured UL_TxConfirmation() */
extern CONST(uint8, ETHIF_APPL_CONST) EthIf_Up_TxConfirmation_MaxNum;

/** \brief Maximum number of configured UL_TrcvLinkStateChg() */
extern CONST(uint8, ETHIF_APPL_CONST) EthIf_Up_TrcvLinkStateChg_MaxNum;

/** \brief Maximum number of configured frame type owner */
extern CONST(uint8, ETHIF_APPL_CONST) EthIf_FrameTypeOwner_MaxNum;

#define ETHIF_STOP_SEC_CONFIG_DATA_8
#include <EthIf_MemMap.h>

#define ETHIF_START_SEC_CONFIG_DATA_16
#include <EthIf_MemMap.h>

/** \brief Declaration of frame type owner lookup table
 *
 * This lookup table contains the relation ship between frame type owner and frame type.
 */
extern CONST(uint16, ETHIF_APPL_CONST) EthIf_LOT_FrameTypeOwnerToFrameType[];

#define ETHIF_STOP_SEC_CONFIG_DATA_16
#include <EthIf_MemMap.h>
/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

#define ETHIF_START_SEC_VAR_INIT_32
#include <EthIf_MemMap.h>

#if (ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON)
extern VAR(uint32, ETHIF_VAR) EthIf_MeasurementDropData;
#endif /* ETHIF_GETANDRESETMEASUREMENTDATA_API == STD_ON */

#define ETHIF_STOP_SEC_VAR_INIT_32
#include <EthIf_MemMap.h>

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/
#define ETHIF_START_SEC_CODE
#include <EthIf_MemMap.h>

/**
 * \brief Rx-Indication dummy callback function
 *
 * This function is used if upper layer does not provide a callback function.
 *
 * \param[in] CtrlIdx - Unused parameter
 * \param[in] FrameType - Unused parameter
 * \param[in] IsBroadcast - Unused parameter
 * \param[in] PhysAddrPtr - Unused parameter
 * \param[in] DataPtr - Unused parameter
 * \param[in] LenByte - Unused parameter
 *
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_Up_RxIndicationDummy
(
  uint8 CtrlIdx,
  Eth_FrameType FrameType,
  boolean IsBroadcast,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) PhysAddrPtr,
  P2VAR(uint8, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  uint16 LenByte
);

/**
 * \brief Tx-Confirmation dummy callback function
 *
 * This function is used if upper layer does not provide a callback function.
 *
 * \param[in] CtrlIdx - Unused parameter
 * \param[in] BufIdx - Unused parameter
 *
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_Up_TxConfirmationDummy
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

/**
 * \brief  Transceiver link state change dummy callback function
 *
 * This function is used if upper layer does not provide a callback function.
 *
 * \param[in] CtrlIdx - Unused parameter
 * \param[in] TransceiverLinkState - Unused parameter
 *
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, ETHIF_CODE) EthIf_Up_TrcvLinkStateChgDummy
(
  uint8 CtrlIdx,
  EthTrcv_LinkStateType TransceiverLinkState
);

#define ETHIF_STOP_SEC_CODE
#include <EthIf_MemMap.h>
/*==================[internal function definitions]=========================*/

#endif /* ifndef ETHIF_INT_H */
/*==================[end of file]===========================================*/
