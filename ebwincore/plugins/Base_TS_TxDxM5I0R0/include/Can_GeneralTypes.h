#ifndef CAN_GENERALTYPES_H
/* !LINKSTO Base.ASR403.CAN439,1 */
#define CAN_GENERALTYPES_H

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/* !LINKSTO Base.ASR403.CAN437,1, Base.ASR413.SWS_Can_00487,1 */

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>        /* AUTOSAR COM stack type definitions */
#include <Can_GeneralTypes_Cfg.h>  /* configuration dependent CAN type definitions */

/*==================[macros]=================================================*/

/*==================[type definitions]=======================================*/

/* !LINKSTO Base.ASR403.CAN436,1, Base.ASR403.CAN438,1 */

/** \brief This type is used to provide ID, DLC and SDU from CAN interface to
 *         CAN driver.
 */
/* !LINKSTO Base.ASR403.CAN415,1 */
typedef struct
{
  P2VAR(uint8, TYPEDEF, AUTOSAR_COMSTACKDATA) sdu;  /**< Pointer to the data */
  Can_IdType id;                                    /**< CAN ID */
  PduIdType swPduHandle;                            /**< PDU ID for Tx confirmation */
  uint8 length;                                     /**< Data length code (DLC) */
} Can_PduType;


/** \brief This type defines a data structure which clearly provides an Hardware Object
 *         Handle including its corresponding CAN Controller and therefore CanDrv as well
 *         as the specific CanId.
 */
/* !LINKSTO Base.ASR413.SWS_Can_00496,1 */
typedef struct
{
  Can_IdType CanId;      /**< Standard/Extended CAN ID of CAN L-PDU */
  Can_HwHandleType Hoh;  /**< ID of the corresponding Hardware Object Range */
  uint8 ControllerId;    /**< ControllerId provided by CanIf clearly identify
                              the corresponding controller */
} Can_HwType;


/** \brief State transitions that are used by the function Can_SetControllerMode. */
/* !LINKSTO Base.ASR403.CAN417,1 */
typedef enum
{
  CAN_T_START,  /**< CAN controller transition value to request state STARTED. */
  CAN_T_STOP,   /**< CAN controller transition value to request state STOPPED. */
  CAN_T_SLEEP,  /**< CAN controller transition value to request state SLEEP. */
  CAN_T_WAKEUP  /**< CAN controller transition value to request state STOPPED from state SLEEP. */
} Can_StateTransitionType;


/** \brief Return values of CAN driver API. */
/* !LINKSTO Base.ASR403.CAN039,1 */
typedef enum
{
  CAN_OK,       /**< Success */
  CAN_NOT_OK,   /**< Error occurred or wakeup event occurred during sleep transition */
  CAN_BUSY      /**< Transmit request could not be processed because no transmit object was
                     available. */
} Can_ReturnType;

/** \brief Error states of a CAN controller. */
/* !LINKSTO Base.ASR430.SWS_Can_91003,1 */
typedef enum
{
  CAN_ERRORSTATE_ACTIVE,    /**< The CAN controller takes fully part in communication. */
  CAN_ERRORSTATE_PASSIVE,   /**< The CAN controller takes  part in communication, but does not send active error frames. */
  CAN_ERRORSTATE_BUSOFF     /**<  The CAN controller does not take part in communication. */
} Can_ErrorStateType;


/* !LINKSTO Base.GeneralTypes.Can_ControllerStateType,1 */
#if (TS_GENERALTYPES_CAN_CONTROLLERSTATETYPE == STD_ON)
/** \brief States that are used by the several ControllerMode functions. */
/* !LINKSTO Base.ASR430.SWS_Can_91013,1 */
typedef enum
{
  CAN_CS_UNINIT = 0x00,  /**< CAN controller state UNINIT */
  CAN_CS_STARTED,        /**< CAN controller state STARTED */
  CAN_CS_STOPPED,        /**< CAN controller state STOPPED */
  CAN_CS_SLEEP           /**< CAN controller state SLEEP */
} Can_ControllerStateType;
#endif /* if (TS_GENERALTYPES_CAN_CONTROLLERSTATETYPE == STD_ON) */

/* CAN Transceiver Driver types */

/** \brief Operating modes of the CAN Transceiver Driver. */
typedef enum
{
  CANTRCV_TRCVMODE_NORMAL = 0,  /**< Transceiver mode NORMAL */
  CANTRCV_TRCVMODE_SLEEP,       /**< Transceiver mode SLEEP */
  CANTRCV_TRCVMODE_STANDBY      /**< Transceiver mode STANDBY */
} CanTrcv_TrcvModeType;

/** \brief This type shall be used to control the CAN transceiver concerning
 *         wake up events and wake up notifications.
 */
typedef enum
{
  CANTRCV_WUMODE_ENABLE = 0,    /**< The notification for wakeup events is enabled on the addressed
                                     network. */
  CANTRCV_WUMODE_DISABLE,       /**< The notification for wakeup events is disabled on the
                                     addressed network. */
  CANTRCV_WUMODE_CLEAR          /**< A stored wakeup event is cleared on the addressed network. */
} CanTrcv_TrcvWakeupModeType;


/** \brief This type denotes the wake up reason detected by the CAN transceiver
 *         in detail.
 */
typedef enum
{
  CANTRCV_WU_ERROR = 0,         /**< Due to an error wake up reason was not detected. This value may
                                     only be reported when error was reported to DEM before. */
  CANTRCV_WU_BY_BUS,            /**< The transceiver has detected, that the network has caused the
                                     wake up of the ECU. */
  CANTRCV_WU_BY_PIN,            /**< The transceiver has detected a wake-up event at one of the
                                     transceiver's pins (not at the CAN bus). */
  CANTRCV_WU_INTERNALLY,        /**< The transceiver has detected, that the network has woken up by
                                     the ECU via a request to NORMAL mode. */
  CANTRCV_WU_NOT_SUPPORTED,     /**< The transceiver does not support any information for the wake
                                     up reason. */
  CANTRCV_WU_POWER_ON,          /**< The transceiver has detected, that the "wake up" is due to an
                                     ECU reset after power on. */
  CANTRCV_WU_RESET,             /**< The transceiver has detected, that the "wake up" is due to an
                                     ECU reset. */
  CANTRCV_WU_BY_SYSERR          /**< The transceiver has detected, that the "wake up" is due to an
                                     HW related device failure. */
} CanTrcv_TrcvWakeupReasonType;


/** \brief Datatype used for describing whether PN wakeup
 *         functionality in CanTrcv is enabled or disabled.
 */
typedef enum
{
  PN_ENABLED = 0,               /**< PN wakeup functionality in CanTrcv is enabled. */
  PN_DISABLED                   /**< PN wakeup functionality in CanTrcv is disabled. */
} CanTrcv_PNActivationType;


/** \brief Provides the state of a flag in the transceiver hardware. */
typedef enum
{
  CANTRCV_FLAG_SET = 0,         /**< The flag is set in the transceiver hardware. */
  CANTRCV_FLAG_CLEARED          /**< The flag is cleared in the transceiver hardware. */
} CanTrcv_TrcvFlagStateType;

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

/*==================[end of file]============================================*/
#endif /* ifndef CAN_GENERALTYPES_H */
