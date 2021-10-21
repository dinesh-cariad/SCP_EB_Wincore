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
[!AUTOSPACING!]
[!INCLUDE "../../generate_macros/CanIf_PreCompileMacros.m"!]
[!INCLUDE "../../generate_macros/CanIf_Macros.m"!]
[!IF "$canifMFSupport"!]
[!INCLUDE "../../generate_macros/CanIf_DecoupledProcessing.m"!]
[!ELSE!]
[!VAR "decoupledRxSupported" = "false()"!]
[!VAR "txDecoupledSupport" = "false()"!]
[!ENDIF!]
#ifndef CANIF_TYPES_H
#define CANIF_TYPES_H

/*==================[inclusions]=============================================*/

#include <TSAutosar.h>
#include <Can_GeneralTypes.h>

[!IF "$wakeupNotificationCalled = 'true'"!]
#include <EcuM_Cbk.h>
[!ENDIF!]

/*==================[macros]=================================================*/

/** \brief Maximum DLC of a CAN 2.0 frame */
#if (defined CANIF_MAXIMUM_CAN_20_DLC)
#error CANIF_MAXIMUM_CAN_20_DLC already defined!
#endif /* #if( defined CANIF_MAXIMUM_CAN_20_DLC)*/
#define CANIF_MAXIMUM_CAN_20_DLC            8U

/** \brief Maximum DLC of a CAN FD frame */
#if (defined CANIF_MAXIMUM_CAN_FD_DLC)
#error CANIF_MAXIMUM_CAN_FD_DLC already defined!
#endif /* #if( defined CANIF_MAXIMUM_CAN_FD_DLC)*/
#define CANIF_MAXIMUM_CAN_FD_DLC           64U

/*==================[type definitions]=======================================*/

/* --- general types --------------------------------------------------- */

/** \brief States for a controller in the CAN interface
 **
 ** CAN controller states: Initially CANIF_CS_UNINIT, then switch between
 ** CANIF_CS_STOPPED, CANIF_CS_STARTED and CANIF_CS_SLEEP.
 */
typedef enum
{
[!IF "($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')"!][!//
  CANIF_CS_UNINIT  = 0U,                /**<Controller is not initialized */
  CANIF_CS_SLEEP   = (uint8)CAN_CS_SLEEP,      /**<Controller is in sleep mode */
  CANIF_CS_STARTED = (uint8)CAN_CS_STARTED,    /**<Controller is active/running */
  CANIF_CS_STOPPED = (uint8)CAN_CS_STOPPED     /**<Controller is stopped */
[!ELSE!][!//
  CANIF_CS_UNINIT  = 0U, /**<Controller is not initialized */
  CANIF_CS_SLEEP,        /**<Controller is in sleep mode */
  CANIF_CS_STARTED,      /**<Controller is active/running */
  CANIF_CS_STOPPED       /**<Controller is stopped */
[!ENDIF!][!// ($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')
} CanIf_ControllerModeType;


/** \brief State transitions for a controller in the CAN interface
 **
 ** Internal mapping of the state transitions in the CAN driver 
 ** which are used to call the Can_SetControllerMode() API.
 */
typedef enum
{
[!IF "($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')"!][!//
  CANIF_T_START  = (uint8)CAN_CS_STARTED,  /**< CAN controller transition value to request state STARTED. */
  CANIF_T_STOP   = (uint8)CAN_CS_STOPPED,  /**< CAN controller transition value to request state STOPPED. */
  CANIF_T_SLEEP  = (uint8)CAN_CS_SLEEP     /**< CAN controller transition value to request state SLEEP. */
[!ELSE!][!//
  CANIF_T_START  = (uint8)CAN_T_START,  /**< CAN controller transition value to request state STARTED. */
  CANIF_T_STOP   = (uint8)CAN_T_STOP,   /**< CAN controller transition value to request state STOPPED. */
  CANIF_T_SLEEP  = (uint8)CAN_T_SLEEP,  /**< CAN controller transition value to request state SLEEP. */
  CANIF_T_WAKEUP = (uint8)CAN_T_WAKEUP  /**< CAN controller transition value to request state STOPPED from state SLEEP. */
[!ENDIF!][!// ($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')
} CanIf_StateTransitionType;

/** \brief Type for PDU channel mode requests
 **
 ** This type defines the possible mode requests that can be stated for a
 ** PDU channel.
 */
typedef enum
{
  CANIF_OFFLINE = 0U,      /**<Transmit and receive path of the corresponding channel are disabled => no communication mode */
  CANIF_TX_OFFLINE,        /**<Transmit path of the corresponding channel is disabled. The receive path is enabled. */
  CANIF_TX_OFFLINE_ACTIVE, /**<Transmit path of the corresponding channel is in offline active mode. The receive path is disabled. */
  CANIF_ONLINE             /**<Transmit and receive path of the corresponding channel are enabled => full operation mode */
}CanIf_PduModeType;


/** \brief Notification status type
 **
 ** This type defines the values that the "read Rx/Tx notification status"
 ** API functions return if they are activated via configuration.
 */
typedef enum
{
  CANIF_NO_NOTIFICATION = 0U,  /**<No new event since the last read attempt*/
  CANIF_TX_RX_NOTIFICATION     /**<New Rx/Tx event since the last read*/
} CanIf_NotifStatusType;

/* --- CAN driver function definitions---------------------------------- */

/** \brief Pointer type definition to function Can_Write()
 **
 ** This is a function pointer type definition for the CAN driver function
 ** Can_Write().
 */
[!IF "($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')"!][!//
typedef P2FUNC( Std_ReturnType, CAN_CODE, CanIf_WriteFctPtrType )
[!ELSE!][!//
typedef P2FUNC( Can_ReturnType, CAN_CODE, CanIf_WriteFctPtrType )
[!ENDIF!][!// ($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')
  (
    Can_HwHandleType Hth,
    const Can_PduType* PduInfo
  );
  
/** \brief Pointer type definition to function Can_SetControllerMode()
 **
 ** This is a function pointer type definition for the CAN driver function
 ** Can_SetControllerMode().
 */
[!IF "($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')"!][!//
typedef P2FUNC( Std_ReturnType, CAN_CODE, CanIf_SetControllerModeFctPtrType )
  (
    uint8 Controller,
    Can_ControllerStateType Transition
  );
[!ELSE!][!//
typedef P2FUNC( Can_ReturnType, CAN_CODE, CanIf_SetControllerModeFctPtrType )
  (
    uint8 Controller,
    Can_StateTransitionType Transition
  );
[!ENDIF!][!// ($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')

[!IF "$canWakeupSupport = 'true'"!]
/** \brief Pointer type definition to function Can_CheckWakeup()
 **
 ** This is a function pointer type definition for the CAN driver function
 ** Can_CheckWakeup().
 */
[!IF "($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')"!][!//
typedef P2FUNC( Std_ReturnType, CAN_CODE, CanIf_CheckWakeupFctPtrType )
[!ELSE!][!//
typedef P2FUNC( Can_ReturnType, CAN_CODE, CanIf_CheckWakeupFctPtrType )
[!ENDIF!][!// ($canDriverCompatibility = 'ASR431') or ($canDriverCompatibility = 'ASR440')
  (
    uint8 Controller
  );

[!ENDIF!][!// $canWakeupSupport = 'true'

[!IF "CanIfPublicCfg/CanIfSetBaudrateApi = 'true'"!]
/** \brief Pointer type definition to function Can_SetBaudrate()
 **
 ** This is a function pointer type definition for the CAN driver function
 ** Can_SetBaudrate().
 */
typedef P2FUNC( Std_ReturnType, CAN_CODE, CanIf_SetBaudrateFctPtrType )
  (
    uint8 Controller,
    uint16 BaudRateConfigID
  );

[!ENDIF!]
[!IF "$canIfBusMirrorSupport = 'true'"!]
  [!IF "$canIfCanGetCntrlStateSupported = 'true'"!]
/** \brief Pointer type definition to function Can_GetControllerErrorState()
 **
 ** This is a function pointer type definition for the CAN driver function
 ** Can_GetControllerErrorState().
 */
typedef P2FUNC( Std_ReturnType, CAN_CODE, CanIf_GetControllerErrorStateFctPtrType )
  (
    uint8 ControllerId,
    Can_ErrorStateType* ErrorStatePtr
  );

  [!ENDIF!]
  [!IF "$canIfCanTxErrCntSupported = 'true'"!]
/** \brief Pointer type definition to function Can_GetControllerTxErrorCounter()
 **
 ** This is a function pointer type definition for the CAN driver function
 ** Can_GetControllerTxErrorCounter().
 */
typedef P2FUNC( Std_ReturnType, CAN_CODE, CanIf_GetControllerTxErrorCounterFctPtrType )
  (
    uint8 ControllerId,
    uint8* TxErrorCounterPtr
  );

  [!ENDIF!]
[!ENDIF!]

/* --- CAN Transceiver Configuration ----------------------------------- */

[!IF "$canTrcvSupport"!]
/** \brief Pointer to function CanTrcv_SetOpMode()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_SetOpMode().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_TrcvSetOpModeFctPtrType )
  (
    uint8 Transceiver,
    CanTrcv_TrcvModeType OpMode
  );


/** \brief Pointer to function CanTrcv_GetOpMode()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_GetOpMode().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_TrcvGetOpModeFctPtrType )
  (
    uint8 Transceiver,
    P2VAR( CanTrcv_TrcvModeType, AUTOMATIC, CANTRCV_APPL_DATA ) OpMode
  );

[!IF "$canTrcvWakeupSupport"!]
/** \brief Pointer to function CanTrcv_GetBusWuReason()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_GetBusWuReason().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_TrcvGetBusWuRFctPtrType )
  (
    uint8 Transceiver,
    P2VAR( CanTrcv_TrcvWakeupReasonType, AUTOMATIC, CANTRCV_APPL_DATA ) Reason
  );


/** \brief Pointer to function CanTrcv_SetWakeupMode()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_SetWakeupMode().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_TrcvSetWuModeFctPtrType )
  (
    uint8 Transceiver,
    CanTrcv_TrcvWakeupModeType TrcvWakeupMode
  );


/** \brief Pointer to function CanTrcv_CB_WakeupByBus()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_CB_WakeupByBus().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_CBWakeupByBusFctPtrType )
  (
    uint8 Transceiver
  );
[!ENDIF!][!// CanTrcv Wakeup support

[!IF "$canifPNSupport"!]
/** \brief Pointer to function CanTrcv_ClearTrcvWufFlag()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_ClearTrcvWufFlag().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_ClearTrcvWufFlagFctPtrType )
  (
    uint8 Transceiver
  );


/** \brief Pointer to function CanTrcv_CheckWakeup()
 **
 ** This is a function pointer type for the CAN transceiver function
 ** CanTrcv_CheckWakeup().
 */
typedef P2FUNC( Std_ReturnType, CANTRCV_CODE, CanIf_CheckTrcvWakeFlagFctPtrType )
  (
    uint8 Transceiver
  );
[!ENDIF!][!// Partial Networking support
[!ENDIF!][!// CanTrcv support


/* --- upper layer callbacks and notifications ------------------------- */


/** \brief Pointer to <UpperLayer>_RxIndication
 **
 ** This is a function pointer type for the RxIndication function
 ** of all well-known upper layers and for all CDDs where parameter
 ** CanIfUpperLayerUseCanId is false.
 */
typedef P2FUNC( void, CANIF_APPL_CODE, CanIf_UlRxIndFctPtrType )
  (
    PduIdType CanIfRxPduId,
    P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) CanIfRxPduPtr
  );


/** \brief Pointer to <CDD>_RxIndication
 **
 ** This is a function pointer type for the RxIndication function
 ** of all CDD upper layers where parameter CanIfUpperLayerUseCanId is true.
 */
typedef P2FUNC( void, CANIF_APPL_CODE, CanIf_CddRxIndFctPtrType )
  (
    PduIdType CanIfRxPduId,
    P2VAR(PduInfoType, AUTOMATIC, CANIF_APPL_DATA) CanIfRxPduPtr,
    Can_IdType CanIfRxCanId
  );


/** \brief Pointer to <UpperLayer>_TxConfirmation
 **
 ** This is a function pointer type for the TxConfirmation function
 ** of all upper layers of CanIf.
 */
 /* !LINKSTO CanIf.EB.User_TxConfirmation.Signature.Result.OFF, 1 */
typedef P2FUNC( void, CANIF_APPL_CODE, CanIf_TxConfFctPtrType )
  (
    PduIdType CanIfTxPduId
  );

[!IF "$ulTxConfirmationResutSupported"!]
/** \brief Pointer to <UpperLayer>_TxConfirmation
 **
 ** This is a function pointer type for the TxConfirmation function
 ** of all upper layers of CanIf.
 */
/* !LINKSTO CanIf.EB.User_TxConfirmation.Signature.Result.ON, 1 */
typedef P2FUNC( void, CANIF_APPL_CODE, CanIf_TxConfFctResultPtrType )
  (
    PduIdType CanIfTxPduId,
    Std_ReturnType result
  );
[!ENDIF!][!// TxConfirmation Resut support

/** \brief Pointer to DLC check notification functions
 **
 ** This is a function pointer type for the upper layer DLC check notification
 ** functions.
 */
typedef P2FUNC( void, CANIF_APPL_CODE, CanIf_DlcCheckNotifFctPtrType )
  (
    PduIdType CanRxPduId
  );


/** \brief Pointer to ActMainFunction functions
 **
 ** This is a function pointer type for the SchM decoupled processing
 ** activation functions.
 */
typedef P2FUNC( void, CANIF_APPL_CODE, CanIf_ActMainFctPtrType )
  (
    void
  );


/* --- CAN driver Configuration ---------------------------------------- */
/** \brief CAN Driver function configuration
 **
 ** This type is used for configuring the CAN Driver list of functions.
 */

typedef struct
{
  CanIf_WriteFctPtrType                        Write;                        /**< Can Write */
  CanIf_SetControllerModeFctPtrType            SetControllerMode;            /**< Can Set Controller Mode */
[!IF "$canWakeupSupport = 'true'"!]
  CanIf_CheckWakeupFctPtrType                  CheckWakeup;                  /**< Can Check Wakeup */
[!ENDIF!]
[!IF "CanIfPublicCfg/CanIfSetBaudrateApi = 'true'"!]
  CanIf_SetBaudrateFctPtrType                  SetBaudrate;                  /**< Can Set Baudrate */
[!ENDIF!]
[!IF "$canIfBusMirrorSupport = 'true'"!]
  [!IF "$canIfCanGetCntrlStateSupported = 'true'"!]
  CanIf_GetControllerErrorStateFctPtrType      GetControllerErrorState;      /**< Can Get Controller Error State */
  [!ENDIF!]
  [!IF "$canIfCanTxErrCntSupported = 'true'"!]
  CanIf_GetControllerTxErrorCounterFctPtrType  GetControllerTxErrorCounter;  /**< Can Get Controller Tx Error Counter */
  [!ENDIF!]
[!ENDIF!]
  Can_HwHandleType                             HohIdOffset;                  /**< Offset for the HW Object id in the local struct */
} CanIf_CanDrvFctConfigType;


/* --- CAN Transceiver Configuration ----------------------------------- */

[!IF "$canTrcvSupport"!]
/** \brief CAN Transceiver Driver function configuration
 **
 ** This type is used for configuring the CAN Transceiver Driver functions.
 */
typedef struct
{
  CanIf_TrcvSetOpModeFctPtrType setOpMode;        /**< set operation mode */
  CanIf_TrcvGetOpModeFctPtrType getOpMode;        /**< get operation mode */
[!IF "$canTrcvWakeupSupport"!]
  CanIf_TrcvGetBusWuRFctPtrType getBusWuReason;   /**< get wakeup reason */
  CanIf_TrcvSetWuModeFctPtrType setWakeupMode;    /**< set wakeup mode */
  CanIf_CBWakeupByBusFctPtrType chkWakeupByBus;   /**< wakeup polling */
[!ENDIF!][!// CanTrcv Wakeup support
[!IF "$canifPNSupport"!]
  CanIf_ClearTrcvWufFlagFctPtrType clearWufFlag;  /**< clear wake flag */
  CanIf_CheckTrcvWakeFlagFctPtrType checkWakeFlag;/**< check wake flag */
[!ENDIF!][!// Partial Networking support
} CanIf_CanTrcvConfigType;
[!ENDIF!][!// CanTrcv support

/* --- Upper Layer Configuration --------------------------------------- */

[!IF "($ulRxIndicationSupport = 'true') or ($cddRxIndicationSupport = 'true') or ($ulTxConfirmationSupport = 'true') or ($dlcErrorNotifSupport = 'true') or ($dlcPassedNotifSupport = 'true')"!]
/** \brief Upper layer callback function configuration
 **
 ** This type contains the function pointer of the API functions
 ** <UL>_RxIndication, <CDD>_RxIndication, <UL>_TxConfirmation,
 ** <UL>_DlcErrorNotification and <UL>_DlcPassedNotification of all
 ** upper layers of the CanIf.
 */
typedef struct
{
[!IF "$ulRxIndicationSupport"!]
  VAR(CanIf_UlRxIndFctPtrType, TYPEDEF)       UlRxIndFctPtr;         /**< <UL>_RxIndication */
[!ENDIF!][!// UL RxIndication support

[!IF "$cddRxIndicationSupport"!]
  VAR(CanIf_CddRxIndFctPtrType, TYPEDEF)      CddRxIndFctPtr;        /**< <Cdd>_RxIndication */
[!ENDIF!][!// Cdd RxIndication support

[!IF "$ulTxConfirmationSupport"!]
  VAR(CanIf_TxConfFctPtrType, TYPEDEF)        TxConfFctPtr;          /**< <UL>_TxConfirmation */
[!IF "$ulTxConfirmationResutSupported"!]
  VAR(CanIf_TxConfFctResultPtrType, TYPEDEF)  TxConfFctResultPtr;    /**< <UL>_TxConfirmation with result supported*/
[!ENDIF!][!// TxConfirmation Resut support
[!ENDIF!][!// TxConfirmation support

[!IF "$dlcErrorNotifSupport"!]
  VAR(CanIf_DlcCheckNotifFctPtrType, TYPEDEF) DlcErrorNotifFctPtr;   /**< <UL>_DlcFailedNotif */
[!ENDIF!]

[!IF "$dlcPassedNotifSupport"!]
  VAR(CanIf_DlcCheckNotifFctPtrType, TYPEDEF) DlcPassedNotifFctPtr;  /**< <UL>_DlcPassedNotif */
[!ENDIF!]
} CanIf_CbkFctPtrTblType;
[!ENDIF!][!// Upper Layer callback support

[!IF "$txDecoupledSupport"!]
/** \brief Tx decoupled main function transmission configuration
 **
 ** This type contains the function pointer of the SchM ActMainFunction API functions,
 ** the queue offset and the size of the queue of all TxPdus that support decoupled transmission.
 */
typedef struct
{
  VAR(CanIf_ActMainFctPtrType, TYPEDEF) ActMainTxFctPtr;  /**< SchM_ActMainFunctionTx */
  VAR([!"$canifTxQOffsetType"!], TYPEDEF) TxQueueOffset;                      /**< Queue offset of the Main Function */
  VAR([!"$canifTxQOffsetType"!], TYPEDEF) TxQueueSize;                        /**< Queue size of the Main Function */
} CanIf_MfTxPtrTblType;
[!ENDIF!][!// Decoupled transmission support

[!IF "$decoupledRxSupported"!]
/** \brief Rx decoupled main function reception configuration
 **
 ** This type contains the function pointer of the SchM ActMainFunction API functions,
 ** the queue offset and the size of the queue of all RxPdus that support decoupled reception.
 */
typedef struct
{
  VAR(CanIf_ActMainFctPtrType, TYPEDEF) ActMainRxFctPtr;  /**< SchM_ActMainFunctionRx */
  VAR([!"$canifMaxPayloadSizeType"!], TYPEDEF) PayloadQueueSize;                  /**< Queue payload data size of the Main Function */
  VAR([!"$canifMaxPayloadSizeTotalType"!], TYPEDEF) PayloadQueueOffset;                /**< Queue payload data offset of the Main Function */
  VAR([!"$canifRxQOffsetType"!], TYPEDEF) RxQueueOffset;                     /**< Queue offset of the Main Function */
  VAR([!"$canifRxPduQueueSizeType"!], TYPEDEF) RxQueueSize;                        /**< Queue size of the Main Function */
} CanIf_MfRxPtrTblType;
[!ENDIF!][!// Decoupled reception support

/* --- Tx buffering ---------------------------------------------------- */

/** \brief Type for Tx buffers
 **
 ** This type is used for the transmit buffers.
 */
typedef struct
{
[!IF "$canifSetDynTxIdSupported or $CanIfMetaDataSupport"!]
  Can_IdType id;                        /**< CAN ID */
[!ENDIF!]
  uint8 length;                         /**< DLC */
} CanIf_TxBufferType;

[!IF "$txDecoupledSupport"!]
  /* --- Tx Pdu control queueing  ---------------------------------------- */

/** \brief Type for Tx control queue
 **
 ** This type is used for the transmit queue.
 */
typedef struct
{
  PduIdType PduId;                     /**< Pdu ID */
  uint8 MFIdx;                         /**< associated Main Function index */
} CanIf_TxQueueType;
[!ENDIF!][!// Decoupled transmission


[!IF "$decoupledRxSupported"!]
/* --- Rx Pdu control queueing  ---------------------------------------- */

/** \brief Type for Rx control queue
 **
 ** This type is used for the receive queue.
 */
typedef struct
{
  Can_IdType CanId;              /**< Can ID needed for CDD upper layers */
  PduIdType PduId;               /**< Pdu ID */
  uint16 PreviousQueuePos;       /**< position of the previous entry in queue */
  uint8 ControllerId;            /**< Id of the associated Controller */
  uint8 MFIdx;                   /**< associated Main Function index */
  uint8 PayloadLen;              /**< Length of payload */
} CanIf_RxQueueType;
[!ENDIF!][!// Decoupled reception

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANIF_TYPES_H */
/*==================[end of file]============================================*/
