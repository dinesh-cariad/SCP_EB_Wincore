#ifndef ETH_API_H
#define ETH_API_H

/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.7 (required)
 *   Expressions resulting from the expansion of macro parameters shall be enclosed in parentheses.
 *
 *   Reason:
 *   The AUTOSAR compiler abstraction requires these definitions in this way. Declarations,
 *   function arguments and function return value cannot be enclosed in parentheses due to C
 *   syntax.
 *
 */

/*==================[inclusions]============================================*/

#include <Std_Types.h>     /* Autosar standard data types */
#include <Eth_Cfg.h>       /* Generated configuration */
#include <Eth_Types.h>     /* Module public types */
#include <Eth_TypesInt.h>  /* Eth configuration types */
/* !LINKSTO Eth.ASR41.SWS_Eth_00006, 1 */
#include <Eth_Version.h>       /* Module Version Info */

#if (ETH_PBCFGM_SUPPORT_ENABLED == STD_ON)
#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* ETH_PBCFGM_SUPPORT_ENABLED */

/*==================[macros]================================================*/

/** \brief Defines the instance number of this Ethernet Driver **/
#define ETH_INSTANCE_ID                   0U

#if (defined ETH_E_INV_CTRL_IDX)
#error ETH_E_INV_CTRL_IDX is already defined
#endif
/** \brief API requests called with invalid controller index */
#define ETH_E_INV_CTRL_IDX                0x01U

#if (defined ETH_E_NOT_INITIALIZED)
#error ETH_E_NOT_INITIALIZED is already defined
#endif
/** \brief API requests called before Eth module is initialized */
#define ETH_E_NOT_INITIALIZED             0x02U

#if (defined ETH_E_INV_POINTER)
#error ETH_E_INV_POINTER is already defined
#endif
/** \brief API requests called with invalid pointer in parameter list */
#define ETH_E_INV_POINTER                 0x03U

#if (defined ETH_E_INV_PARAM)
#error ETH_E_INV_PARAM is already defined
#endif
/** \brief API requests called with invalid parameter */
#define ETH_E_INV_PARAM                   0x04U

#if (defined ETH_E_INV_CONFIG)
#error ETH_E_INV_CONFIG is already defined
#endif
/** \brief API requests called with invalid configuration */
#define ETH_E_INV_CONFIG                  0x05U

#if (defined ETH_E_INV_MODE)
#error ETH_E_INV_MODE is already defined
#endif
/** \brief API requests called with invalid mode */
#define ETH_E_INV_MODE                    0x06U

#if (defined ETH_E_FRAMES_LOST)
#error ETH_E_FRAMES_LOST is already defined
#endif
/** \brief Eth_Receive detects a lost of frame */
#define ETH_E_FRAMES_LOST                 0x07U

#if (defined ETH_E_CRTL_ACCESS_FAILING)
#error ETH_E_CRTL_ACCESS_FAILING is already defined
#endif
/** \brief Eth_ControllerInit detects access to eth controller is failing */
#define ETH_E_CRTL_ACCESS_FAILING         0x80U

#if (defined ETH_E_CRTL_MII_MNG_FAILING)
#error ETH_E_CRTL_MII_MNG_FAILING is already defined
#endif
/** \brief Eth_ReadMii/WriteMii access is failing (timeout) */
#define ETH_E_CRTL_MII_MNG_FAILING        0x81U

#if (defined ETH_E_PHYSADDRFILTER_POOL_FULL)
#error ETH_E_PHYSADDRFILTER_POOL_FULL is already defined
#endif
/** \brief Physical address filter pool is full */
#define ETH_E_PHYSADDRFILTER_POOL_FULL    0x82U

#if (defined ETH_E_DMA_BUSY)
#error ETH_E_DMA_BUSY is already defined
#endif
/** \brief DMA is still busy */
#define ETH_E_DMA_BUSY                  0x83U

#if (defined ETH_E_BUFFER_INVALID)
#error ETH_E_BUFFER_INVALID is already defined
#endif
/** \brief Buffer is invalid */
#define ETH_E_BUFFER_INVALID            0x84U

#if (defined ETH_E_DEVICE_MEMORY_MAPPING_INVALID)
#error ETH_E_DEVICE_MEMORY_MAPPING_INVALID is already defined
#endif
/** \brief Buffer is invalid */
#define ETH_E_DEVICE_MEMORY_MAPPING_INVALID 0x85U

#if (defined ETH_E_API_NOT_SUPPORTED)
#error ETH_E_API_NOT_SUPPORTED is already defined
#endif
/** \brief Buffer is invalid */
#define ETH_E_API_NOT_SUPPORTED         0x86U

#if (defined ETH_E_VIRTIO_ERROR)
#error ETH_E_VIRTIO_ERROR is already defined
#endif
/** \brief Buffer is invalid */
#define ETH_E_VIRTIO_ERROR             0x87U

#if (defined ETH_E_BUF_NOT_ALIGNED)
#error ETH_E_BUF_NOT_ALIGNED is already defined
#endif
/** \brief Buffer is invalid */
#define ETH_E_BUF_NOT_ALIGNED            0x88U

/** \brief Defines API id of function Eth_Init().
 **/
#define ETH_INIT_SVCID                    0x01U

/* !LINKSTO Eth.EB.Eth_ControllerInit, 1 */
/** \brief Defines API id of function Eth_ControllerInit().
 **/
#define ETH_CONTROLLERINIT_SVCID          0x02U

/** \brief Defines API id of function Eth_SetControllerMode().
 **/
#define ETH_SETCONTROLLERMODE_SVCID       0x03U

/** \brief Defines API id of function Eth_GetControllerMode().
 **/
#define ETH_GETCONTROLLERMODE_SVCID       0x04U

/** \brief Defines API id of function Eth_WriteMii().
 **/
#define ETH_WRITEMII_SVCID                0x05U

/** \brief Defines API id of function Eth_ReadMii().
 **/
#define ETH_READMII_SVCID                 0x06U

/** \brief Defines API id of function Eth_GetCounterState().
 **/
#define ETH_GETCOUNTERSTATE_SVCID         0x07U

/** \brief Defines API id of function Eth_GetPhysAddr().
 **/
#define ETH_GETPHYSADDR_SVCID             0x08U

/** \brief Defines API id of function Eth_ProvideTxBuffer().
 **/
#define ETH_PROVIDETXBUFFER_SVCID         0x09U

/** \brief Defines API id of function Eth_Transmit().
 **/
#define ETH_TRANSMIT_SVCID                0x0AU

/** \brief Defines API id of function Eth_Transmit().
 **/
#define ETH_RECEIVE_SVCID                 0x0BU

/** \brief Defines API id of function Eth_TxConfirmation().
 **/
#define ETH_TXCONFIRMATION_SVCID          0x0CU

/** \brief Defines API id of function Eth_RxIrqHdlr_<CtrlIdx>().
 **/
#define ETH_RXIRQHDLR_SVCID               0x10U

/** \brief Defines API id of function Eth_TxIrqHdlr_<CtrlIdx>().
 **/
#define ETH_TXIRQHDLR_SVCID               0x11U

/** \brief Defines API id of function Eth_UpdatePhysAddrFilter().
 **/
#define ETH_UPDATEPHYSADDRFILTER_SVCID    0x12U

/** \brief Defines API id of function Eth_SetPhysAddr().
 **/
#define ETH_SETPHYSADDR_SVCID             0x13U

/** \brief Defines API id of function Eth_GetCounterValues().
 **/
#define ETH_GETCOUNTERVALUES_SVCID        0x14U

/** \brief Defines API id of function Eth_GetRxStats().
 **/
#define ETH_GETRXSTATS_SVCID             0x15U

/** \brief Defines API id of function Eth_GetTxStats().
 **/
#define ETH_GETTXSTATS_SVCID             0x1cU

/** \brief Defines API id of function Eth_GetTxErrorCounterValues().
 **/
#define ETH_GETTXERRORCOUNTERVALUES_SVCID 0x1dU

/** \brief Defines API id of function Eth_GetVersionInfo().
 **/
#define ETH_GETVERSIONINFO_SVCID          0x0DU

/** \brief Defines API id of function Eth_GetCurrentTime().
 **/
#define ETH_GETCURRENTTIME_SVCID          0x16U

/** \brief Defines API id of function Eth_EnableEgressTimeStamp().
 **/
#define ETH_ENABLEEGRESSTIMESTAMP_SVCID   0x17U

/** \brief Defines API id of function Eth_GetEgressTimeStamp().
 **/
#define ETH_GETEGRESSTIMESTAMP_SVCID      0x18U

/** \brief Defines API id of function Eth_GetIngressTimeStamp().
 **/
#define ETH_GETINGRESSTIMESTAMP_SVCID     0x19U

/** \brief Defines API id of function Eth_SetCorrectionTime().
 **/
#define ETH_SETCORRECTIONTIME_SVCID       0x1AU

/** \brief Defines API id of function Eth_SetGlobalTime().
 **/
#define ETH_SETGLOBALTIME_SVCID           0x1BU



/** \brief Defines API id of internal Eth functions.
 **/
#define ETH_INTERNAL_SVCID                0x82U

/* !LINKSTO Eth.EB.Eth_MiiInit, 1 */
/** \brief Defines API id of function Eth_MiiInit().
 **/
#define ETH_MIIINIT_SVCID                 0x83U

/* !LINKSTO Eth.EB.Eth_Retransmit, 1 */
/** \brief Defines API id of function Eth_Retransmit().
 **/
#define ETH_RETRANSMIT_SVCID              0x84U

/* !LINKSTO Eth.EB.Eth_TransmitBufList, 1 */
/** \brief Defines API id of function Eth_TransmitBufList().
 **/
#define ETH_TRANSMITBUFLIST_SVCID         0x85U

/** \brief Defines API id of function Eth_SetRxInterruptUsercalloutMode().
 **/
#define ETH_SETRXINTERRUPTUSERCALLOUTMODE_SVCID    0x86U

/** \brief Defines API id of function Eth_SetTxInterruptUsercalloutMode().
 **/
#define ETH_SETTXINTERRUPTUSERCALLOUTMODE_SVCID    0x87U


/* Note: ETH_EB_CONTROLLERINIT function macro will be used by EB template.
   Predefined here, this macro can be defined outside of the Eth module by the user.
 */
#ifndef ETH_EB_CONTROLLERINIT

#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422) || (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)
/** \brief helper function for ETH_EB_CONTROLLERINIT function macro.
 * One or two function parameters according to the Autosar version.
 */
#define ETH_EB_CONTROLLERINIT_FUNC( CtrlIdx )  Eth_ControllerInit( CtrlIdx, 0U )
#else
#define ETH_EB_CONTROLLERINIT_FUNC( CtrlIdx )  Eth_ControllerInit( CtrlIdx)
#endif

/** \brief Initialization of the Eth controller. Function macro will be used by EB template.
 * It is pre-defined here and can be defined outside of the Eth module by the user.
 */
#define ETH_EB_CONTROLLERINIT() \
  do \
  { \
    uint8 CtrlIdx; \
    for( CtrlIdx = 0U; CtrlIdx < ETH_CTRL_NO; CtrlIdx++ ) \
    { \
      if( ETH_EB_CONTROLLERINIT_FUNC( CtrlIdx ) != E_OK) \
      { \
        /* Note: handle error here or cast to (void) */ \
      } \
    } \
  } \
  while( 0U )

#endif /* ETH_EB_CONTROLLERINIT */

/* Note: ETH_EB_MIIINIT function macro will be used by EB template.
   Predefined here, this macro can be defined outside of the Eth module by the user.
 */
#ifndef ETH_EB_MIIINIT

/** \brief Initialization of the Eth controller's MII. Function macro will be used by EB template.
 * It is pre-defined here and can be defined outside of the Eth module by the user.
 */
#define ETH_EB_MIIINIT() \
  do \
  { \
    uint8 CtrlIdx; \
    for( CtrlIdx = 0U; CtrlIdx < ETH_CTRL_NO; CtrlIdx++ ) \
    { \
      if( Eth_MiiInit( CtrlIdx ) != E_OK) \
      { \
        /* Note: handle error or cast to (void) */ \
      } \
    } \
  } \
  while( 0U )

#endif /* ETH_EB_MIIINIT */

/*==================[macro checks]==========================================*/

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define ETH_START_SEC_CODE
#include <Eth_MemMap.h>

/** \brief Validate configuration
 **
 ** Checks if the post build configuration fits to the link time configuration part.
 **
 ** \return E_OK if the given module configurations is valid otherwise E_NOT_OK.
 **
 ** \ServiceID{0x60}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, ETH_CODE) Eth_IsValidConfig(
    P2CONST(void, AUTOMATIC, ETH_APPL_CONST) voidConfigPtr);


extern FUNC(void, ETH_CODE) Eth_Init
(
  P2CONST(Eth_ConfigType, AUTOMATIC, ETH_APPL_CONST) PBCfgPtr
);

#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422) || (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)
/* !LINKSTO Eth.EB.Eth_ControllerInit, 1 */
extern FUNC(Std_ReturnType, ETH_CODE) Eth_ControllerInit
(
  uint8 CtrlIdx,
  uint8 CfgIdx
);
#else /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)||(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)*/
/* !LINKSTO Eth.EB.Eth_ControllerInit, 1 */
extern FUNC(Std_ReturnType, ETH_CODE) Eth_ControllerInit
(
  uint8 CtrlIdx
);
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)||(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR430A)*/

extern FUNC(Std_ReturnType, ETH_CODE) Eth_SetControllerMode
(
  uint8 CtrlIdx,
  Eth_ModeType CtrlMode
);

extern FUNC(Std_ReturnType, ETH_CODE) Eth_GetControllerMode
(
  uint8 CtrlIdx,
  P2VAR(Eth_ModeType, AUTOMATIC, ETH_APPL_DATA) CtrlModePtr
);

extern FUNC(void, ETH_CODE) Eth_GetPhysAddr
(
  uint8 CtrlIdx,
  P2VAR(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

extern FUNC(void, ETH_CODE) Eth_SetPhysAddr
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

#if (ETH_UPDATE_PHYS_ADDR_FILTER_SUPPORT == STD_ON)
/**
 ** \brief   Update physical address filter.
 **
 ** Updates pool of allowed multicast physical addresses for receive and enables/disables
 ** promiscuous mode of the indexed controller. Physical addresses used for promiscuous mode
 ** enabling/disabling are FF:FF:FF:FF:FF:FF and 00:00:00:00:00:00 with behaviour as following:
 **
 **  MAC:                    Action:                     Behaviour:
 **  FF:FF:FF:FF:FF:FF       ETH_ADD_TO_FILTER           PROMISCUOUS MODE ON
 **  FF:FF:FF:FF:FF:FF       ETH_REMOVE_FROM_FILTER      PROMISCUOUS MODE OFF
 **  00:00:00:00:00:00       ETH_ADD_TO_FILTER           PROMISCUOUS MODE OFF + POOL FLUSH
 **  00:00:00:00:00:00       ETH_REMOVE_FROM_FILTER      PROMISCUOUS MODE OFF + POOL FLUSH
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[in] PhysAddrPtr        Pointer to memory containing the physical source address
 **                               (MAC address) in network byte order.
 ** \param[in] Action             ETH_ADD_TO_FILTER or ETH_REMOVE_FROM_FILTER.
 **
 ** \return     E_OK : successful
 **             E_NOT_OK : failed
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Non reentrant for the same CtrlIdx or reentrant for different CtrlIdx}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETH_CODE) Eth_UpdatePhysAddrFilter
(
  uint8 CtrlIdx,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr,
  Eth_FilterActionType Action
);
#endif /* ETH_UPDATE_PHYS_ADDR_FILTER_SUPPORT == STD_ON */

#if (ETH_ENABLE_MII_SUPPORT == STD_ON)
/* !LINKSTO Eth.EB.Eth_MiiInit, 1 */
extern FUNC(Std_ReturnType, ETH_CODE) Eth_MiiInit
(
  uint8 CtrlIdx
);

extern FUNC(Eth_ReturnType, ETH_CODE) Eth_WriteMii
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  uint16 RegVal
);

extern FUNC(Eth_ReturnType, ETH_CODE) Eth_ReadMii
(
  uint8 CtrlIdx,
  uint8 TrcvIdx,
  uint8 RegIdx,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) RegValPtr
);
#endif /* ETH_ENABLE_MII_SUPPORT == STD_ON) */

extern FUNC(void, ETH_CODE) Eth_GetCounterState
(
  uint8 CtrlIdx,
  uint16 CtrlOffs,
  P2VAR(uint32, AUTOMATIC, ETH_APPL_DATA) CtrlValPtr
);

extern FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetCounterValues
(
  uint8 CtrlIdx,
  P2VAR(Eth_CounterType, AUTOMATIC, EBSTUBS_APPL_DATA) CounterPtr
);

extern FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetRxStats
(
  uint8 CtrlIdx,
  P2VAR(Eth_RxStatsType, AUTOMATIC, EBSTUBS_APPL_DATA) RxStats
);

extern FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetTxStats
(
  uint8 CtrlIdx,
  P2VAR(Eth_TxStatsType, AUTOMATIC, EBSTUBS_APPL_DATA) TxStats
);

extern FUNC(Std_ReturnType, EBSTUBS_CODE) Eth_GetTxErrorCounterValues
(
  uint8 CtrlIdx,
  P2VAR(Eth_TxErrorCounterValuesType, AUTOMATIC, EBSTUBS_APPL_DATA) TxErrorCounterValues
);

#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)
/* Deviation MISRAC2012-1 <START> */
extern FUNC(BufReq_ReturnType, ETH_CODE) Eth_ProvideTxBuffer
(
  uint8 CtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
);
/* Deviation MISRAC2012-1 <STOP> */
#else /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/
/* Deviation MISRAC2012-1 <START> */
extern FUNC(BufReq_ReturnType, ETH_CODE) Eth_ProvideTxBuffer
(
  uint8 CtrlIdx,
  uint8 Priority,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(P2VAR(Eth_DataType, TYPEDEF, ETH_APPL_DATA), AUTOMATIC, ETH_APPL_DATA) BufPtr,
  P2VAR(uint16, AUTOMATIC, ETH_APPL_DATA) LenBytePtr
);
/* Deviation MISRAC2012-1 <STOP> */
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/

extern FUNC(Std_ReturnType, ETH_CODE) Eth_Transmit
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  Eth_FrameType FrameType,
  boolean TxConfirmation,
  uint16 LenByte,
  P2CONST(uint8, AUTOMATIC, ETH_APPL_DATA) PhysAddrPtr
);

#if (ETH_SUPPORT_RETRANSMIT == STD_ON)
/**
 ** \brief   Retransmit a received/transmitted frame
 **
 ** This function retransmits the current buffer. This function can be called inside
 ** EthIfRx_Indication() function (OrigCtrlIdx and DataPtr of current buffer must be provided, the
 ** BufIdxPtr must point the value ETH_BUFIDXTYPE_INVALID) and inside the EthIf_TxConfirmation()
 ** function (OrigCtrlIdx and BufIdxPtr of current buffer must be provided).
 ** The current buffer is not released when the related EthIf_RxIndication function or
 ** EthIf_TxConfirmation function returns. In both cases the function returns an buffer index
 ** (parameter BufIdxPtr).
 ** If TxConfirmation is true the Eth driver calls EthIf_TxConfirmation() function with this buffer
 ** index after transmission.
 ** This function returns E_NOT_OK if it is not called in a related EthIf_RxIndication() or
 ** EthIf_TxConfirmation() function.
 **
 ** \param[in] CtrlIdx         Index of the controller where frame shall be re-transmit
 ** \param[in] OrigCtrlIdx     Index of the controller of the provided buffer
 ** \param[in,out] BufIdxPtr   Buffer index of the retransmitted frame (in case of calling inside
 **                            EthIfRx_Indication() the buffer index must be ETH_BUFIDXTYPE_INVALID)
 ** \param[in] DataPtr         Buffer pointer of retransmitted frame (only in case of
 **                            EthIf_RxIndication context) else it is NULL_PTR
 ** \param[in] FrameType       Ethernet type of the related payload for the transmission.
 ** \param[in] LenByte         Length of the payload contained in the tx buffer to transmit.
 ** \param[in] RetransmitInfoPtr  Pointer to additonal retransmit info.
 ** \return E_OK The re-transmission of the buffer was successful
 ** \return E_NOT_OK The re-transmission of the buffer was not successful
 **
 ** \ServiceID{0x84}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETHIF_CODE) Eth_Retransmit
(
  uint8 CtrlIdx,
  uint8 OrigCtrlIdx,
  P2VAR(Eth_BufIdxType, AUTOMATIC, ETH_APPL_DATA) BufIdxPtr,
  P2VAR(Eth_DataType, AUTOMATIC, ETHIF_APPL_DATA) DataPtr,
  Eth_FrameType FrameType,
  uint16 LenByte,
  CONSTP2CONST(Eth_RetransmitInfoType, AUTOMATIC, ETH_APPL_DATA) RetransmitInfoPtr
);
#endif /* (ETH_SUPPORT_RETRANSMIT == STD_ON) */

#if (ETH_BUFFER_LIST_SUPPORT == STD_ON)
/**
 ** \brief   Ethernet transmit buffer list
 **
 ** Transmit a single Ethernet frame with the provided buffer list. The provided buffers must also
 ** contain the Ethernet frame header (destination MAC, source MAC, EtherType).
 ** The ownership of the provided BufList itself and the linked buffers goes to the Eth driver
 ** till the EthIf_TxConfirmationBufList() for this transmission is called.
 **
 ** When the frame is transmitted the function EthIf_TxConfirmationBufList() is called
 ** with the provided UserPtr. This returns the ownership of the BufList and the buffers
 ** back to the user.
 **
 ** \param[in] CtrlIdx     Index of the addresses Eth controller.
 ** \param[in] Priority    Frame priority for transmit buffer FIFO selection
 ** \param[in] BufListPtr  Pointer to the provided buffer list.
 ** \param[in] BufListNo   Number of buffers in the provided buffer list.
 ** \param[in] UserPtr     User pointer.
 **
 ** \return     E_OK : successful
 **             E_NOT_OK : failed
 **
 ** \ServiceID{0x85}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
FUNC(Std_ReturnType, ETH_CODE) Eth_TransmitBufList
(
  const uint8 CtrlIdx,
  const uint8 Priority,
  CONSTP2CONST(Eth_BufListType, TYPEDEF, ETH_APPL_DATA) BufListPtr,
  const uint8 BufListNo,
  CONSTP2CONST(uint8, TYPEDEF, ETH_APPL_DATA) UserPtr
);
#endif /* (ETH_BUFFER_LIST_SUPPORT == STD_ON) */

#if (ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)
extern FUNC(void, ETH_CODE) Eth_Receive
(
  uint8 CtrlIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
);
#else /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/
extern FUNC(void, ETH_CODE) Eth_Receive
(
  uint8 CtrlIdx,
  uint8 FifoIdx,
  P2VAR(Eth_RxStatusType, AUTOMATIC, ETH_APPL_DATA) RxStatus
);
#endif /*(ETH_SUPPORT_API == ETH_SUPPORT_API_ASR422)*/

extern FUNC(void, ETH_CODE) Eth_TxConfirmation
(
  uint8 CtrlIdx
);

#if (ETH_GET_VERSION_INFO == STD_ON)
extern FUNC(void, ETH_CODE) Eth_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, ETH_APPL_DATA) VersionInfoPtr
);
#endif

#if (ETH_GLOBAL_TIME_SUPPORT == STD_ON)
/**
 ** \brief   Provide current time
 **
 ** Returns a time value according its definition out of the HW registers.
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[out] timeQualPtr       Quality of HW time stamp, e.g. based on current drift
 ** \param[out] timeStampPtr      current time stamp
 **
 ** \return     E_OK : successful
 **             E_NOT_OK : failed
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETH_CODE) Eth_GetCurrentTime
(
  uint8 CtrlIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) timeStampPtr
);

/**
 ** \brief   Enable to generate the egress time stamp
 **
 ** Activates egress time stamping on a dedicated message object.
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[in] BufIdx             Index of the message buffer, where application expects egress
 **                               time stamping
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, ETH_CODE) Eth_EnableEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx
);

/**
 ** \brief   Provide egress time stamp of a message object
 **
 ** Reads back the egress time stamp on a dedicated message object.
 ** Must be called within the the context of the TxConfirmation function.
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[in] BufIdx             Index of the message buffer, where application expects egress
 **                               time stamping
 ** \param[out] timeQualPtr       Quality of HW time stamp, e.g. based on current drift
 ** \param[out] timeStampPtr      Current time stamp
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, ETH_CODE) Eth_GetEgressTimeStamp
(
  uint8 CtrlIdx,
  Eth_BufIdxType BufIdx,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) timeStampPtr
);

/**
 ** \brief   Provide ingress time stamp of a message object
 **
 ** Reads back the ingress time stamp on a dedicated message object.
 ** Must be called within the RxIndication function.
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[in] DataPtr            Pointer to the message buffer, where application expects ingress
 **                               time stamping
 ** \param[out] timeQualPtr       Quality of HW time stamp, e.g. based on current drift
 ** \param[out] timeStampPtr      Current time stamp
 **
 ** \ServiceID{0x13}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, ETH_CODE) Eth_GetIngressTimeStamp
(
  uint8 CtrlIdx,
  P2CONST(Eth_DataType, AUTOMATIC, ETH_APPL_DATA) DataPtr,
  P2VAR(Eth_TimeStampQualType, AUTOMATIC, ETH_APPL_DATA) timeQualPtr,
  P2VAR(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) timeStampPtr
);

/**
 ** \brief   Set of the local time
 **
 ** Allows the Time Slave to adjust the local ETH Reference clock in HW.
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[in,out] timeOffsetPtr  Offset between time stamp grandmaster and time stamp by local clock
 **                               (OriginTimeStampSync minus IngressTimeStampSync) + Pdelay
 ** \param[in,out] rateRatioPtr   Time elements to calculate and to modify the ratio of the frequency
 **                               of the grandmaster in relation to the frequency of the local clock
 **                               with: ratio = OriginTimeStampDelta / IngressTimeStampDelta
 **
 ** \ServiceID{0x19}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, ETH_CODE) Eth_SetCorrectionTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeIntDiffType, AUTOMATIC, ETH_APPL_DATA) timeOffsetPtr,
  P2CONST(Eth_RateRatioType, AUTOMATIC, ETH_APPL_DATA) rateRatioPtr
);

/**
 ** \brief   Set of global time on the HW
 **
 ** Allows the Time Master to adjust the global ETH Reference clock in HW.
 ** We can use this method to set a global time base on ETH in general or to synchronize the global
 ** ETH time base with another time base, e.g. FlexRay.
 **
 ** \param[in] CtrlIdx            Index of the addresses ETH controller.
 ** \param[in] timeStampPtr       New time stamp
 **
 ** \ServiceID{0x1A}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, ETH_CODE) Eth_SetGlobalTime
(
  uint8 CtrlIdx,
  P2CONST(Eth_TimeStampType, AUTOMATIC, ETH_APPL_DATA) timeStampPtr
);

#endif /* (ETH_GLOBAL_TIME_SUPPORT == STD_ON) */



#if (ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT == STD_ON)
/**
 ** \brief Ethernet set Rx interrupt user callout mode
 **
 ** The function enables (Mode == ETH_MODE_ACTIVE) or disables (Mode == ETH_MODE_DOWN) the user
 ** callout for the Rx interrupt.
 **
 ** \ServiceID{0x86}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, ETH_CODE) Eth_SetRxInterruptUsercalloutMode
(
  const uint8 CtrlIdx,
  const uint8 FifoIdx,
  const Eth_ModeType Mode
);
#endif /* (ETH_ENABLE_RX_IRQ_CALLOUT_SUPPORT == STD_ON) */

#if (ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT == STD_ON)
/**
 ** \brief Ethernet set Tx interrupt user callout mode
 **
 ** The function enables (Mode == ETH_MODE_ACTIVE) or disables (Mode == ETH_MODE_DOWN) the user
 ** callout for the Tx interrupt.
 **
 ** \ServiceID{0x87}
 ** \Reentrancy{Non Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, ETH_CODE) Eth_SetTxInterruptUsercalloutMode
(
  const uint8 CtrlIdx,
  const uint8 FifoIdx,
  const Eth_ModeType Mode
);
#endif /* (ETH_ENABLE_TX_IRQ_CALLOUT_SUPPORT == STD_ON) */


#define ETH_STOP_SEC_CODE
#include <Eth_MemMap.h>

#include <Eth_WinPcap_Api.h>  /* additional Eth Api for WinPcap driver */


/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef ETH_API_H */
/*==================[end of file]===========================================*/

