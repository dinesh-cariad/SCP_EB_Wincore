/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANIF_API_H
#define CANIF_API_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>          /* EB specific standard types */
#include <ComStack_Types.h>     /* AUTOSAR COM types */
#include <CanIf_Version.h>      /* CanIf version declaration */
#include <CanIf_Cfg.h>          /* CanIf configuration */
#include <CanIf_Types.h>        /* CanIf type definitions */
#include <CanIf_Types_Int.h>    /* CanIf PostBuild type definitions */
#if (CANIF_PBCFGM_SUPPORT_ENABLED == STD_ON)

#include <PbcfgM_Api.h>         /* Post build configuration manager */
#endif /* CANIF_PBCFGM_SUPPORT_ENABLED */

#if(CANIF_DECOUPLED_PROCESSING_SUPPORT == STD_ON)
#include <CanIf_MultiMF.h>
#endif /* CANIF_DECOUPLED_PROCESSING_SUPPORT */
/*==================[macros]=================================================*/

/*------------------[API service IDs]----------------------------------------*/

/** \brief CanIf_Init() service ID */
#if (defined CANIF_SID_INIT)
#error CANIF_SID_INIT already defined!
#endif /* #if( defined CANIF_SID_INIT)*/
#define CANIF_SID_INIT                      0x01U

/** \brief CanIf_SetControllerMode() service ID */
#if (defined CANIF_SID_SETCONTROLLERMODE)
#error CANIF_SID_SETCONTROLLERMODE already defined!
#endif /* #if( defined CANIF_SID_SETCONTROLLERMODE)*/
#define CANIF_SID_SETCONTROLLERMODE         0x03U

/** \brief CanIf_GetControllerMode() service ID */
#if (defined CANIF_SID_GETCONTROLLERMODE)
#error CANIF_SID_GETCONTROLLERMODE already defined!
#endif /* #if( defined CANIF_SID_GETCONTROLLERMODE)*/
#define CANIF_SID_GETCONTROLLERMODE         0x04U

/** \brief CanIf_Transmit() service ID */
#if (defined CANIF_SID_TRANSMIT)
#error CANIF_SID_TRANSMIT already defined!
#endif /* #if( defined CANIF_SID_TRANSMIT)*/
#define CANIF_SID_TRANSMIT                  0x05U

/** \brief CanIf_CancelTransmit() service ID */
#if (defined CANIF_SID_CANCELTRANSMIT)
#error CANIF_SID_CANCELTRANSMIT already defined!
#endif /* #if( defined CANIF_SID_CANCELTRANSMIT)*/
#define CANIF_SID_CANCELTRANSMIT            0x18U

/** \brief CanIf_ReadRxPduData() service ID */
#if (defined CANIF_SID_READRXPDUDATA)
#error CANIF_SID_READRXPDUDATA already defined!
#endif /* #if( defined CANIF_SID_READRXPDUDATA)*/
#define CANIF_SID_READRXPDUDATA             0x06U

/** \brief CanIf_ReadTxNotifStatus() service ID */
#if (defined CANIF_SID_READTXNOTIFSTATUS)
#error CANIF_SID_READTXNOTIFSTATUS already defined!
#endif /* #if( defined CANIF_SID_READTXNOTIFSTATUS)*/
#define CANIF_SID_READTXNOTIFSTATUS         0x07U

/** \brief CanIf_ReadRxNotifStatus() service ID */
#if (defined CANIF_SID_READRXNOTIFSTATUS)
#error CANIF_SID_READRXNOTIFSTATUS already defined!
#endif /* #if( defined CANIF_SID_READRXNOTIFSTATUS)*/
#define CANIF_SID_READRXNOTIFSTATUS         0x08U

/** \brief CanIf_SetPduMode() service ID */
#if (defined CANIF_SID_SETPDUMODE)
#error CANIF_SID_SETPDUMODE already defined!
#endif /* #if( defined CANIF_SID_SETPDUMODE)*/
#define CANIF_SID_SETPDUMODE                0x09U

/** \brief CanIf_GetPduMode() service ID */
#if (defined CANIF_SID_GETPDUMODE)
#error CANIF_SID_GETPDUMODE already defined!
#endif /* #if( defined CANIF_SID_GETPDUMODE)*/
#define CANIF_SID_GETPDUMODE                0x0aU

/** \brief CanIf_GetVersionInfo() service ID */
#if (defined CANIF_SID_GETVERSIONINFO)
#error CANIF_SID_GETVERSIONINFO already defined!
#endif /* #if( defined CANIF_SID_GETVERSIONINFO)*/
#define CANIF_SID_GETVERSIONINFO            0x0bU

/** \brief CanIf_SetDynamicTxId() service ID */
#if (defined CANIF_SID_SETDYNAMICTXID)
#error CANIF_SID_SETDYNAMICTXID already defined!
#endif /* #if( defined CANIF_SID_SETDYNAMICTXID)*/
#define CANIF_SID_SETDYNAMICTXID            0x0cU

/** \brief CanIf_SetTrcvMode() service ID */
#if (defined CANIF_SID_SETTRANSCEIVERMODE)
#error CANIF_SID_SETTRANSCEIVERMODE already defined!
#endif /* #if( defined CANIF_SID_SETTRANSCEIVERMODE)*/
#define CANIF_SID_SETTRANSCEIVERMODE        0x0dU

/** \brief CanIf_GetTrcvMode() service ID */
#if (defined CANIF_SID_GETTRANSCEIVERMODE)
#error CANIF_SID_GETTRANSCEIVERMODE already defined!
#endif /* #if( defined CANIF_SID_GETTRANSCEIVERMODE)*/
#define CANIF_SID_GETTRANSCEIVERMODE        0x0eU

/** \brief CanIf_GetTrcvWakeupReason() service ID */
#if (defined CANIF_SID_GETTRCVWAKEUPREASON)
#error CANIF_SID_GETTRCVWAKEUPREASON already defined!
#endif /* #if( defined CANIF_SID_GETTRCVWAKEUPREASON)*/
#define CANIF_SID_GETTRCVWAKEUPREASON       0x0fU

/** \brief CanIf_SetTrcvWakeupMode() service ID */
#if (defined CANIF_SID_SETTRCVWAKEUPMODE)
#error CANIF_SID_SETTRCVWAKEUPMODE already defined!
#endif /* #if( defined CANIF_SID_SETTRCVWAKEUPMODE)*/
#define CANIF_SID_SETTRCVWAKEUPMODE         0x10U

/** \brief CanIf_CheckWakeup() service ID */
#if (defined CANIF_SID_CHECKWAKEUP)
#error CANIF_SID_CHECKWAKEUP already defined!
#endif /* #if( defined CANIF_SID_CHECKWAKEUP)*/
#define CANIF_SID_CHECKWAKEUP               0x11U

/** \brief CanIf_CheckValidation() service ID */
#if (defined CANIF_SID_CHECKVALIDATION)
#error CANIF_SID_CHECKVALIDATION already defined!
#endif /* #if( defined CANIF_SID_CHECKVALIDATION)*/
#define CANIF_SID_CHECKVALIDATION           0x12U

/** \brief CanIf_GetTxConfirmationState() service ID */
#if (defined CANIF_SID_GETTXCONFIRMSTATE)
#error CANIF_SID_GETTXCONFIRMSTATE already defined!
#endif /* #if( defined CANIF_SID_GETTXCONFIRMSTATE)*/
#define CANIF_SID_GETTXCONFIRMSTATE         0x19U

/** \brief CanIf_ClearTrcvWufFlag() service ID */
#if (defined CANIF_SID_CLEARTRCVWUFFLAG)
#error CANIF_SID_CLEARTRCVWUFFLAG already defined!
#endif /* #if( defined CANIF_SID_CLEARTRCVWUFFLAG)*/
#define CANIF_SID_CLEARTRCVWUFFLAG          0x1eU

/** \brief CanIf_CheckTrcvWakeFlag() service ID */
#if (defined CANIF_SID_CHECKTRCVWAKEFLAG)
#error CANIF_SID_CHECKTRCVWAKEFLAG already defined!
#endif /* #if( defined CANIF_SID_CHECKTRCVWAKEFLAG)*/
#define CANIF_SID_CHECKTRCVWAKEFLAG         0x1fU

/** \brief CanIf_TxConfirmation() service ID */
#if (defined CANIF_SID_TXCONFIRMATION)
#error CANIF_SID_TXCONFIRMATION already defined!
#endif /* #if( defined CANIF_SID_TXCONFIRMATION)*/
#define CANIF_SID_TXCONFIRMATION            0x13U

/** \brief CanIf_RxIndication() service ID */
#if (defined CANIF_SID_RXINDICATION)
#error CANIF_SID_RXINDICATION already defined!
#endif /* #if( defined CANIF_SID_RXINDICATION)*/
#define CANIF_SID_RXINDICATION              0x14U

/** \brief CanIf_CancelTxConfirmation() service ID */
#if (defined CANIF_SID_CANCELTXCONFIRMATION)
#error CANIF_SID_CANCELTXCONFIRMATION already defined!
#endif /* #if( defined CANIF_SID_CANCELTXCONFIRMATION)*/
#define CANIF_SID_CANCELTXCONFIRMATION      0x15U

/** \brief CanIf_ControllerBusOff() service ID */
#if (defined CANIF_SID_CONTROLLERBUSOFF)
#error CANIF_SID_CONTROLLERBUSOFF already defined!
#endif /* #if( defined CANIF_SID_CONTROLLERBUSOFF)*/
#define CANIF_SID_CONTROLLERBUSOFF          0x16U

/** \brief CanIf_ControllerModeIndication() service ID */
#if (defined CANIF_SID_CONTROLLERMODEIND)
#error CANIF_SID_CONTROLLERMODEIND already defined!
#endif /* #if( defined CANIF_SID_CONTROLLERMODEIND)*/
#define CANIF_SID_CONTROLLERMODEIND         0x17U

/** \brief CanIf_TrcvModeIndication() service ID */
#if (defined CANIF_SID_TRANSCEIVERMODEIND)
#error CANIF_SID_TRANSCEIVERMODEIND already defined!
#endif /* #if( defined CANIF_SID_TRANSCEIVERMODEIND)*/
#define CANIF_SID_TRANSCEIVERMODEIND        0x18U

/** \brief CanIf_ConfirmPnAvailability() service ID */
#if (defined CANIF_SID_CONFIRMPNAVAILABILITY)
#error CANIF_SID_CONFIRMPNAVAILABILITY already defined!
#endif /* #if( defined CANIF_SID_CONFIRMPNAVAILABILITY)*/
#define CANIF_SID_CONFIRMPNAVAILABILITY     0x1aU

/** \brief CanIf_ClearTrcvWufFlagIndication() service ID */
#if (defined CANIF_SID_CLEARTRCVWUFFLAGIND)
#error CANIF_SID_CLEARTRCVWUFFLAGIND already defined!
#endif /* #if( defined CANIF_SID_CLEARTRCVWUFFLAGIND)*/
#define CANIF_SID_CLEARTRCVWUFFLAGIND       0x20U

/** \brief CanIf_CheckTrcvWakeFlagIndication() service ID */
#if (defined CANIF_SID_CHECKTRCVWAKEFLAGIND)
#error CANIF_SID_CHECKTRCVWAKEFLAGIND already defined!
#endif /* #if( defined CANIF_SID_CHECKTRCVWAKEFLAGIND)*/
#define CANIF_SID_CHECKTRCVWAKEFLAGIND      0x21U

/** \brief CanIf_SetBaudrate() service ID */
#if (defined CANIF_SID_SETBAUDRATE)
#error CANIF_SID_SETBAUDRATE already defined!
#endif /* #if( defined CANIF_SID_SETBAUDRATE)*/
#define CANIF_SID_SETBAUDRATE               0x27U

/** \brief */
#if (defined CANIF_SID_ENABLEBUSMIRRORING)
#error CANIF_SID_ENABLEBUSMIRRORING already defined !
#endif /* ( defined CANIF_SID_ENABLEBUSMIRRORING ) */
#define CANIF_SID_ENABLEBUSMIRRORING        0x4cU

#if (defined CANIF_SID_GETCONTROLLERERRORSTATE)
#error CANIF_SID_GETCONTROLLERERRORSTATE already defined !
#endif
#define CANIF_SID_GETCONTROLLERERRORSTATE   0x4bU

#if (defined CANIF_SID_GETCONTROLLERTXERRORCOUNTER)
#error CANIF_SID_GETCONTROLLERTXERRORCOUNTER already defined !
#endif
#define CANIF_SID_GETCONTROLLERTXERRORCOUNTER 0x4eU
/*------------------[Development error codes]--------------------------------*/

/** \brief CANIF_E_PARAM_CANID */
#if (defined CANIF_E_PARAM_CANID)
#error CANIF_E_PARAM_CANID already defined!
#endif /* #if( defined CANIF_E_PARAM_CANID)*/
#define CANIF_E_PARAM_CANID                 10U

/** \brief CANIF_E_PARAM_DLC */
#if (defined CANIF_E_PARAM_DLC)
#error CANIF_E_PARAM_DLC already defined!
#endif /* #if( defined CANIF_E_PARAM_DLC)*/
#define CANIF_E_PARAM_DLC                   11U

/** \brief CANIF_E_PARAM_HRH */
#if (defined CANIF_E_PARAM_HRH)
#error CANIF_E_PARAM_HRH already defined!
#endif /* #if( defined CANIF_E_PARAM_HRH)*/
#define CANIF_E_PARAM_HRH                   12U

/** \brief CANIF_E_PARAM_LPDU */
#if (defined CANIF_E_PARAM_LPDU)
#error CANIF_E_PARAM_LPDU already defined!
#endif /* #if( defined CANIF_E_PARAM_LPDU)*/
#define CANIF_E_PARAM_LPDU                  13U

/** \brief CANIF_E_PARAM_CONTROLLER */
#if (defined CANIF_E_PARAM_CONTROLLER)
#error CANIF_E_PARAM_CONTROLLER already defined!
#endif /* #if( defined CANIF_E_PARAM_CONTROLLER)*/
#define CANIF_E_PARAM_CONTROLLER            14U

/** \brief CANIF_E_PARAM_CONTROLLERID */
#if (defined CANIF_E_PARAM_CONTROLLERID)
#error CANIF_E_PARAM_CONTROLLERID already defined!
#endif /* #if( defined CANIF_E_PARAM_CONTROLLERID)*/
#define CANIF_E_PARAM_CONTROLLERID          15U

/** \brief CANIF_E_PARAM_WAKEUPSOURCE */
#if (defined CANIF_E_PARAM_WAKEUPSOURCE)
#error CANIF_E_PARAM_WAKEUPSOURCE already defined!
#endif /* #if( defined CANIF_E_PARAM_WAKEUPSOURCE)*/
#define CANIF_E_PARAM_WAKEUPSOURCE          16U

/** \brief CANIF_E_PARAM_TRCV */
#if (defined CANIF_E_PARAM_TRCV)
#error CANIF_E_PARAM_TRCV already defined!
#endif /* #if( defined CANIF_E_PARAM_TRCV)*/
#define CANIF_E_PARAM_TRCV                  17U

/** \brief CANIF_E_PARAM_TRCVMODE */
#if (defined CANIF_E_PARAM_TRCVMODE)
#error CANIF_E_PARAM_TRCVMODE already defined!
#endif /* #if( defined CANIF_E_PARAM_TRCVMODE)*/
#define CANIF_E_PARAM_TRCVMODE              18U

/** \brief CANIF_E_PARAM_TRCVWAKEUPMODE */
#if (defined CANIF_E_PARAM_TRCVWAKEUPMODE)
#error CANIF_E_PARAM_TRCVWAKEUPMODE already defined!
#endif /* #if( defined CANIF_E_PARAM_TRCVWAKEUPMODE)*/
#define CANIF_E_PARAM_TRCVWAKEUPMODE        19U

/** \brief CANIF_E_PARAM_POINTER */
#if (defined CANIF_E_PARAM_POINTER)
#error CANIF_E_PARAM_POINTER already defined!
#endif /* #if( defined CANIF_E_PARAM_POINTER)*/
#define CANIF_E_PARAM_POINTER               20U

/** \brief CANIF_E_PARAM_CTRLMODE */
#if (defined CANIF_E_PARAM_CTRLMODE)
#error CANIF_E_PARAM_CTRLMODE already defined!
#endif /* #if( defined CANIF_E_PARAM_CTRLMODE)*/
#define CANIF_E_PARAM_CTRLMODE              21U

/** \brief CANIF_E_PARAM_PDU_MODE */
#if (defined CANIF_E_PARAM_PDU_MODE)
#error CANIF_E_PARAM_PDU_MODE already defined!
#endif /* #if( defined CANIF_E_PARAM_PDU_MODE)*/
#define CANIF_E_PARAM_PDU_MODE               22U

/** \brief CANIF_E_UNINIT */
#if (defined CANIF_E_UNINIT)
#error CANIF_E_UNINIT already defined!
#endif /* #if( defined CANIF_E_UNINIT)*/
#define CANIF_E_UNINIT                      30U

/** \brief CANIF_E_NOK_NOSUPPORT */
#if (defined CANIF_E_NOK_NOSUPPORT)
#error CANIF_E_NOK_NOSUPPORT already defined!
#endif /* #if( defined CANIF_E_NOK_NOSUPPORT)*/
#define CANIF_E_NOK_NOSUPPORT               40U

/** \brief CANIF_E_INVALID_TXPDUID */
#if (defined CANIF_E_INVALID_TXPDUID)
#error CANIF_E_INVALID_TXPDUID already defined!
#endif /* #if( defined CANIF_E_INVALID_TXPDUID)*/
#define CANIF_E_INVALID_TXPDUID             50U

/** \brief CANIF_E_INVALID_RXPDUID */
#if (defined CANIF_E_INVALID_RXPDUID)
#error CANIF_E_INVALID_RXPDUID already defined!
#endif /* #if( defined CANIF_E_INVALID_RXPDUID)*/
#define CANIF_E_INVALID_RXPDUID             60U


/*------------------[Development runtime error codes]--------------------------------*/

/** \brief CANIF_E_TXPDU_LENGTH_EXCEEDED */
#if (defined CANIF_E_TXPDU_LENGTH_EXCEEDED)
#error CANIF_E_TXPDU_LENGTH_EXCEEDED already defined!
#endif /* #if( defined CANIF_E_TXPDU_LENGTH_EXCEEDED)*/
#define CANIF_E_TXPDU_LENGTH_EXCEEDED       90U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

#if CANIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANIF_START_SEC_CODE_CC_BLOCK
#else
#define CANIF_START_SEC_CODE
#endif
#include <CanIf_MemMap.h>

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
extern FUNC( Std_ReturnType, CANIF_CODE) CanIf_IsValidConfig
  (
    P2CONST(void,AUTOMATIC,CANIF_APPL_CONST) voidConfigPtr
  );

#if CANIF_ISVALIDCONFIG_MMAP_ENABLED == STD_ON
#define CANIF_STOP_SEC_CODE_CC_BLOCK
#else
#define CANIF_STOP_SEC_CODE
#endif
#include <CanIf_MemMap.h>

#define CANIF_START_SEC_CODE
#include <CanIf_MemMap.h>

/** \brief CAN interface initialization function
 **
 ** This function initializes the CAN interface.
 **
 ** Caution:
 ** - CanIf_Init() shall not preempt any other CanIf function.
 ** - No other CanIf function shall interrupt CanIf_Init().
 **
 ** \param[in] ConfigPtr Pointer to the interface configuration
 **
 ** \ServiceID{0x01}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( void, CANIF_CODE ) CanIf_Init
  (
    P2CONST( CanIf_ConfigType, AUTOMATIC, CANIF_APPL_CONST ) ConfigPtr
  );



/** \brief Controller mode setting function
 **
 ** This function performs a mode transition of the controller given in \p
 ** ControllerId to the mode \p ControllerMode
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p ControllerId must address a valid controller
 ** - The parameter \p ControllerMode must be a valid mode
 **
 ** \param[in] ControllerId   Target controller
 ** \param[in] ControllerMode Requested mode transition
 **
 ** \return Result of operation
 ** \retval E_OK      Controller mode request has been accepted
 ** \retval E_NOT_OK  Controller mode request has not been accepted
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetControllerMode
  (
    uint8 ControllerId,
    CanIf_ControllerModeType ControllerMode
  );



/** \brief Get controller mode
 **
 ** This function queries the mode of the controller given in \p ControllerId.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p ControllerId must address a valid controller
 ** - The parameter \p ControllerModePtr must be a valid pointer
 **
 ** \param[in]  ControllerId      CAN controller
 ** \param[out] ControllerModePtr Pointer for returning the current mode
 **
 ** \return Result of operation
 ** \retval E_OK      The returned mode is valid
 ** \retval E_NOT_OK  An error occurred during function execution
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetControllerMode
  (
    uint8 ControllerId,
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
    P2VAR( Can_ControllerStateType, AUTOMATIC, CANIF_APPL_DATA )
#else
    P2VAR( CanIf_ControllerModeType, AUTOMATIC, CANIF_APPL_DATA )
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
      ControllerModePtr
  );



/** \brief Transmit an L-PDU
 **
 ** This function transmits the data given through \p PduInfoPtr through the
 ** L-PDU given by \p CanTxPduId.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p CanTxPduId must be a valid Tx L-PDU
 ** - The parameter \p PduInfoPtr must be a valid pointer
 **
 ** \param[in] CanTxPduId Tx L-PDU handle
 ** \param[in] PduInfoPtr Message content to be transmitted
 **
 ** \return Result of operation
 ** \retval E_OK      Transmit request has been accepted
 ** \retval E_NOT_OK  Transmit request has not been accepted
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_Transmit
  (
    PduIdType CanTxPduId,
    P2CONST( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  );


#if( CANIF_SET_BAUDRATE_API == STD_ON )
/** \brief Set Baudrate API function
 **
 ** This service shall set the baud rate configuration of the CAN controller.
 ** Depending on necessary baud rate modifications the controller might have to
 ** reset.
 **
 ** \param[in] ControllerId CAN controller, whose baud rate shall be set
 ** \param[in] BaudRateConfigID references a baud rate configuration by ID
 **
 ** \return Result of operation
 ** \retval E_OK      Service request accepted, setting of (new) baud rate started
 ** \retval E_NOT_OK  Service request not accepted
 **
 ** \ServiceID{0x27}
 ** \Reentrancy{Reentrant for different Controllers}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetBaudrate
  (
    uint8 ControllerId,
    uint16 BaudRateConfigID
  );
#endif /* CANIF_SET_BAUDRATE_API == STD_ON */


#if( CANIF_CANCEL_TRANSMIT_SUPPORT == STD_ON )
/** \brief Cancel transmit dummy function
 **
 ** This function has no functionality and is called by the AUTOSAR PduR to
 ** achieve bus agnostic behavior.
 **
 ** Preconditions:
 ** - The parameter \p CanTxPduId must be a valid Tx L-PDU
 **
 ** \param[in] CanTxPduId Tx L-PDU handle
 **
 ** \return Always return E_OK
 **
 ** \ServiceID{0x18}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CancelTransmit
  (
    PduIdType CanTxPduId
  );
#endif /* CANIF_CANCEL_TRANSMIT_SUPPORT == STD_ON */



#if( CANIF_READRXPDU_DATA_API == STD_ON )
/** \brief Read received data
 **
 ** This function reads data previously received and stored in an internal
 ** buffer.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - This API must be enabled by configuration
 ** - The parameter \p CanRxPduId must be a valid Rx L-PDU
 ** - The parameter \p PduInfoPtr must be a valid pointer
 **
 ** \param[in]  CanRxPduId  Rx L-PDU handle
 ** \param[out] PduInfoPtr  Memory pointer to store received data
 **
 ** \return Result of operation
 ** \retval E_OK      Request has been accepted
 ** \retval E_NOT_OK  Request has not been accepted
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_ReadRxPduData
  (
    PduIdType CanRxPduId,
    P2VAR( PduInfoType, AUTOMATIC, CANIF_APPL_DATA ) PduInfoPtr
  );
#endif /* CANIF_READRXPDU_DATA_API == STD_ON */



#if( CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON )
/** \brief Read Tx notification status
 **
 ** This function provides the Tx L-PDU transmit notification status of L-PDU
 ** \p CanTxPduId.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - Transmit notification API must be enabled
 ** - The parameter \p CanTxPduId must be a valid Tx L-PDU.
 **
 ** \param[in] CanTxPduId Tx L-PDU handle
 **
 ** \return Current notification status of the corresponding Tx L-PDU
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( CanIf_NotifStatusType, CANIF_CODE ) CanIf_ReadTxNotifStatus
  ( PduIdType CanTxPduId );
#endif /* CANIF_READTXPDU_NOTIF_STATUS_API == STD_ON */



#if( CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON )
/** \brief Read Rx notification status
 **
 ** This function provides the Rx L-PDU receive notification status of L-PDU
 ** \p CanRxPduId.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - Transmit notification API must be enabled
 ** - The parameter \p CanRxPduId must be a valid Rx L-PDU.
 **
 ** \param[in] CanRxPduId Rx L-PDU handle
 **
 ** \return Current notification status of the corresponding Rx L-PDU
 **
 ** \ServiceID{0x08}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( CanIf_NotifStatusType, CANIF_CODE ) CanIf_ReadRxNotifStatus
  ( PduIdType CanRxPduId );
#endif /* CANIF_READRXPDU_NOTIF_STATUS_API == STD_ON */



/** \brief Set requested L-PDU channel mode
 **
 ** This function sets the L-PDU channel mode for the requested controller (\p
 ** ControllerId) to the requested mode (\p PduModeRequest).
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p ControllerId must be a valid CAN controller.
 ** - The parameter \p PduModeRequest must be a valid PDU mode.
 **
 ** \param[in] ControllerId   CAN controller
 ** \param[in] PduModeRequest Requested PDU mode
 **
 ** \return Result of operation
 ** \retval E_OK      Request for mode transition has been accepted
 ** \retval E_NOT_OK  Request for mode transition has not been accepted
 **
 ** \ServiceID{0x09}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetPduMode
  (
    uint8 ControllerId,
    CanIf_PduModeType PduModeRequest
  );



/** \brief Read a L-PDU channel mode
 **
 ** This function returns the current PDU mode of the requested controller
 ** (\p ControllerId) at the memory location referenced by \p PduModePtr.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - The parameter \p ControllerId must be a valid CAN controller
 ** - The parameter \p PduModePtr must be a valid pointer
 **
 ** \param[in]  ControllerId CAN controller
 ** \param[out] PduModePtr Pointer to return the current mode
 **
 ** \return Result of operation
 ** \retval E_OK      Channel mode request has been accepted
 ** \retval E_NOT_OK  Channel mode request has not been accepted
 **
 ** \ServiceID{0x0a}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetPduMode
  (
    uint8 ControllerId,
    P2VAR( CanIf_PduModeType, AUTOMATIC, CANIF_APPL_DATA ) PduModePtr
  );


#if( CANIF_VERSION_INFO_API == STD_ON )
/** \brief Return module version information
 **
 ** This function returns the CAN interface version information in the memory
 ** area \p Versioninfo references.
 **
 ** Preconditions:
 ** - The parameter \p Versioninfo may not be a NULL pointer
 **
 ** \param[out] Versioninfo Version information
 **
 ** \ServiceID{0x0b}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( void, CANIF_CODE ) CanIf_GetVersionInfo
  ( P2VAR( Std_VersionInfoType, AUTOMATIC, CANIF_APPL_DATA ) Versioninfo );
#endif


#if( CANIF_SETDYNAMICTXID_API == STD_ON )
/** \brief Set CAN ID of dynamic Tx L-PDU
 **
 ** This function sets the CAN ID of Tx L-PDU \p CanTxPduId to the new value
 ** \p CanId.
 **
 ** Preconditions:
 ** - The CAN interface must already be initialized by CanIf_Init()
 ** - Dynamic Tx L-PDUs are used in the current configuration
 ** - The parameter \p CanTxPduId is a valid dynamic Tx L-PDU
 ** - The parameter \p CanId is a valid CAN ID
 **
 ** \param[in] CanTxPduId Tx L-PDU handle
 ** \param[in] CanId New CAN ID
 **
 ** \ServiceID{0x0c}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( void, CANIF_CODE ) CanIf_SetDynamicTxId
  (
    PduIdType CanTxPduId,
    Can_IdType CanId
  );
#endif /* CANIF_SETDYNAMICTXID_API == STD_ON */



#if( CANIF_TRCV_SUPPORT == STD_ON )

/** \brief Set transceiver mode
 **
 ** This function is used to set the mode of the transceiver assigned to
 ** transceiver channel \a TransceiverId to the mode given in
 ** \a TransceiverMode.
 **
 ** \param[in] TransceiverId CAN transceiver ID
 ** \param[in] TransceiverMode Requested transceiver mode
 **
 ** \return Result of operation
 ** \retval E_OK Transceiver mode was changed as requested
 ** \retval E_NOT_OK Transceiver mode change failed; previous mode is still
 **         valid
 **
 ** \ServiceID{0x0d}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetTrcvMode
  (
    uint8 TransceiverId,
    CanTrcv_TrcvModeType TransceiverMode
  );



/** \brief Get transceiver mode
 **
 ** This function is used to read the mode of the transceiver assigned to
 ** transceiver channel \a TransceiverId to the memory location
 ** \a TransceiverModePtr.
 **
 ** \param[in] TransceiverModePtr Pointer to mode variable
 ** \param[in] TransceiverId CAN transceiver ID
 **
 ** \return Result of operation
 ** \retval E_OK Transceiver mode was changed as requested
 ** \retval E_NOT_OK Transceiver mode change failed; previous mode is still
 **         valid
 **
 ** \ServiceID{0x0e}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetTrcvMode
  (
#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
    uint8 TransceiverId,
    P2VAR( CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_DATA )
      TransceiverModePtr
#else
    P2VAR( CanTrcv_TrcvModeType, AUTOMATIC, CANIF_APPL_DATA )
      TransceiverModePtr,
    uint8 TransceiverId
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */
  );

#if (CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON)

/** \brief Get transceiver wakeup reason
 **
 ** This function is used to read the last wakeup reason of the transceiver
 ** assigned to transceiver channel \a TransceiverId into \a TrcvWuReasonPtr.
 **
 ** \param[in] TransceiverId CAN transceiver channel ID
 ** \param[in] TrcvWuReasonPtr Address to write wakeup reason to
 **
 ** \return Result of operation
 ** \retval E_OK Wakeup reason was correctly read
 ** \retval E_NOT_OK Wakeup reason could not be read
 **
 ** \ServiceID{0x0f}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_GetTrcvWakeupReason
  (
    uint8 TransceiverId,
    P2VAR( CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANIF_APPL_DATA )
      TrcvWuReasonPtr
  );


/** \brief Set transceiver wakeup mode
 **
 ** This function is used to set the wakeup mode/event \a TrcvWakeupMode of
 ** the transceiver assigned to transceiver channel \a TransceiverId.
 **
 ** \param[in] TransceiverId CAN transceiver channel ID
 ** \param[in] TrcvWakeupMode Wakeup mode/event to set
 **
 ** \return Result of operation
 ** \retval E_OK Wakeup state has been changed
 ** \retval E_NOT_OK Wakeup state change has failed
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_SetTrcvWakeupMode
  (
    uint8 TransceiverId,
    CanTrcv_TrcvWakeupModeType TrcvWakeupMode
  );
#endif /* CANIF_CANTRCV_WAKEUP_SUPPORT == STD_ON */
#endif /* CANIF_TRCV_SUPPORT == STD_ON */


#if( CANIF_WAKEUP_SUPPORT_API == STD_ON )
/** \brief Check for Can and CanTrcv wakeups
 **
 ** This function is used to check whether an underlying CAN driver or CAN
 ** transceiver driver signals a wakeup event.
 **
 ** \param[in] WakeupSource Bitfield of wakeup sources to be checked
 **
 ** \return Result of operation
 ** \retval E_OK The validation request was accepted and executed
 ** \retval E_NOT_OK The validation request was not accepted
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckWakeup
  (
    EcuM_WakeupSourceType WakeupSource
  );


#if( CANIF_WAKEUP_VALIDATION_API == STD_ON )
/** \brief Check for validated wakeup events
 **
 ** This function is used to validate previous wakeup events.
 **
 ** \param[in] WakeupSource Bitfield of wakeup sources to be validated
 **
 ** \return Result of operation
 ** \retval E_OK The validation request was accepted and executed
 ** \retval E_NOT_OK The validation request was not accepted
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckValidation
  (
    EcuM_WakeupSourceType WakeupSource
  );
#endif /* CANIF_WAKEUP_VALIDATION_API == STD_ON */
#endif /* CANIF_WAKEUP_SUPPORT_API == STD_ON */


#if( CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON )
/** \brief Report controller Tx confirmation state
 **
 ** This function reports, if any Tx confirmation has been received for the
 ** whole CAN controller since the last controller start.
 **
 ** Preconditions:
 ** - The parameter \p ControllerId must be a valid CanIf controller ID
 **
 ** \param[in] ControllerId Abstracted CanIf controller ID which is assigned
 **                         to the CAN controller
 **
 ** \return Notification status of the controller
 ** \retval CANIF_NO_NOTIFICATION     No notification was received
 ** \retval CANIF_TX_RX_NOTIFICATION  A Tx confirmation was received
 **
 ** \ServiceID{0x19}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( CanIf_NotifStatusType, CANIF_CODE ) CanIf_GetTxConfirmationState
  (
    uint8 ControllerId
  );
#endif /* CANIF_PUBLIC_TXCONFIRM_POLLING_SUPPORT == STD_ON */

#if ((CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_TRCV_SUPPORT == STD_ON ))
/** \brief Clear WUF flag of the designated CAN transceiver
 **
 ** \param[in] TransceiverId  designated CAN transceiver
 ** \return Result of operation
 ** \retval E_OK      Request has been accepted
 ** \retval E_NOT_OK  Request has not been accepted
 **
 ** \ServiceId{0x1e}
 ** \Reentrancy{Reentrant for different CAN transceiver}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_ClearTrcvWufFlag
  (
    uint8 TransceiverId
  );

/** \brief Check the wake flag of the designated CAN transceiver
 **
 ** \param[in] TransceiverId  designated CAN transceiver
 ** \return Result of operation
 ** \retval E_OK      Request has been accepted
 ** \retval E_NOT_OK  Request has not been accepted
 **
 ** \ServiceId{0x1f}
 ** \Reentrancy{Reentrant for different CAN transceiver}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE ) CanIf_CheckTrcvWakeFlag
  (
    uint8 TransceiverId
  );
#endif /* #if (CANIF_PUBLIC_PN_SUPPORT == STD_ON) && (CANIF_TRCV_SUPPORT == STD_ON )) */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/** \brief Controls the mirroring on a controller
 **
 ** \param[in] ControllerId     Abstracted CanIf ControllerId which is assigned to a
 **                             CAN controller.
 ** \param[in] MirroringActive  TRUE: Mirror_ReportCanFrame will be called for
 **                                   each frame received or transmitted on the given controller.
 **                             FALSE: Mirror_ReportCanFrame will not be called for the given controller.
 ** \return Result of the operation
 ** \retval E_OK                       Mirroring mode was changed.
 ** \retval E_NOT_OK                   Wrong ControllerId.
 **
 ** \ServiceId{0x4c}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC( Std_ReturnType, CANIF_CODE) CanIf_EnableBusMirroring
  (
    uint8 ControllerId,
    boolean MirroringActive
  );

/** \brief Returns the error state of the controller
 **
 ** \param[in] ControllerId     Abstracted CanIf ControllerId which is assigned to a
 **                             CAN controller.
 ** \param[out] ErrorStatePtr   Pointer to a memory location, where the error state
 **                             of the CAN controller will be stored.
 ** \return Result of the operation
 ** \retval E_OK                Error state request has been accepted or always if
 **                             CanIfCanControllerErrorStateSupported is FALSE.
 ** \retval E_NOT_OK            Error state request has not been accepted. This is never
 **                             returned if CanIfCanControllerErrorStateSupported is FALSE.
 **
 ** \ServiceId{0x4b}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC( Std_ReturnType, CANIF_CODE) CanIf_GetControllerErrorState
  (
    uint8 ControllerId,
    Can_ErrorStateType * ErrorStatePtr
  );

/** \brief Returns the TX error counter
 **
 ** \param[in] ControllerId        Abstracted CanIf ControllerId which is assigned to a
    **                             CAN controller.
 ** \param[out] TxErrorCounterPtr  Pointer to a memory location, where the current Tx
 **                                error counter of the CAN controller will be stored.
 ** \return Result of the operation
 ** \retval E_OK                Error state request has been accepted or always if
 **                             CanIfCanTxErrorCounterSupported is FALSE.
 ** \retval E_NOT_OK            Error state request has not been accepted. This is never
 **                             returned if CanIfCanTxErrorCounterSupported is FALSE.
 **
 ** \ServiceId{0x4b}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC (Std_ReturnType, CANIF_CODE) CanIf_GetControllerTxErrorCounter
  (
    uint8 ControllerId,
    uint8* TxErrorCounterPtr
  );

#endif

#define CANIF_STOP_SEC_CODE
#include <CanIf_MemMap.h>

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANIF_API_H */
/*==================[end of file]============================================*/
