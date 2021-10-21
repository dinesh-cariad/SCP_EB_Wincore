/**
 * \file
 *
 * \brief AUTOSAR CanSM
 *
 * This file contains the implementation of the AUTOSAR
 * module CanSM.
 *
 * \version 3.7.5
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANSM_INT_STC_H
#define CANSM_INT_STC_H

/*==================[inclusions]============================================*/

#include <ComStack_Types.h>    /* AUTOSAR communication stack types */
#include <ComM.h>              /* Communication Manager types */
#include <CanSM_Types_Int.h>

/*==================[macros]================================================*/

/*------------------[CanSM macros]-----------*/

#if (defined CANSM_GLOBAL_UNINIT)      /* To prevent double declaration */
#error CANSM_GLOBAL_UNINIT already defined!
#endif /* #if (defined CANSM_GLOBAL_UNINIT) */

/** \brief represents global uninitialized state */
#define CANSM_GLOBAL_UNINIT         0U

#if (defined CANSM_GLOBAL_INIT)      /* To prevent double declaration */
#error CANSM_GLOBAL_INIT already defined!
#endif /* #if (defined CANSM_GLOBAL_INIT) */

/** \brief represents global initialized state */
#define CANSM_GLOBAL_INIT           1U

#if (defined CANSM_NO_BUS_OFF)      /* To prevent double declaration */
#error CANSM_NO_BUS_OFF already defined!
#endif /* #if (defined CANSM_NO_BUS_OFF) */

/** \brief no bus-off event occurred */
#define CANSM_NO_BUS_OFF            0U

#if (defined CANSM_BUS_OFF)      /* To prevent double declaration */
#error CANSM_BUS_OFF already defined!
#endif /* #if (defined CANSM_BUS_OFF) */

/** \brief bus-off event occurred */
#define CANSM_BUS_OFF               1U

#if (defined CANSM_TRANSCEIVER_ID_NONE)      /* To prevent double declaration */
#error CANSM_TRANSCEIVER_ID_NONE already defined!
#endif /* #if (defined CANSM_TRANSCEIVER_ID_NONE) */

/** \brief transceiver id indicating that no transceiver is present */
#define CANSM_TRANSCEIVER_ID_NONE 255U

#if (defined CANSM_INVALID_DEM_EVENTID)      /* To prevent double declaration */
#error CANSM_INVALID_DEM_EVENTID already defined!
#endif /* #if (defined CANSM_INVALID_DEM_EVENTID) */

/** \brief special value: no Dem event id configured (0 is not a valid value for Dem_EventIdType)
 */
#define CANSM_INVALID_DEM_EVENTID   0U

#if (defined CANSM_NO_STATE_REQ)      /* To prevent double declaration */
#error CANSM_NO_STATE_REQ already defined!
#endif /* #if (defined CANSM_NO_STATE_REQ) */

/** \brief indicates that no recovery shall be performed */
#define CANSM_NO_STATE_REQ          0U

#if (defined CANSM_REQ_STATE_FULL_COM)      /* To prevent double declaration */
#error CANSM_REQ_STATE_FULL_COM already defined!
#endif /* #if (defined CANSM_REQ_STATE_FULL_COM) */

/** \brief indicates that recovery to mode FULL_COMMUNICATION shall be performed */
#define CANSM_REQ_STATE_FULL_COM    1U

#if (defined CANSM_REQ_STATE_SILENT_COM)      /* To prevent double declaration */
#error CANSM_REQ_STATE_SILENT_COM already defined!
#endif /* #if (defined CANSM_REQ_STATE_SILENT_COM) */

/** \brief indicates that recovery to mode SILENT_COMMUNICATION shall be performed */
#define CANSM_REQ_STATE_SILENT_COM  2U


/*------------------[Service Ids]-----------*/

#if (defined CANSM_SID_INIT)      /* To prevent double declaration */
#error CANSM_SID_INIT already defined!
#endif /* #if (defined CANSM_SID_INIT) */

/** \brief Service Id of CanSM_Init() */
#define CANSM_SID_INIT 0x00U

#if (defined CANSM_SID_GETVERSIONINFO)      /* To prevent double declaration */
#error CANSM_SID_GETVERSIONINFO already defined!
#endif /* #if (defined CANSM_SID_GETVERSIONINFO) */

/** \brief Service Id of CanSM_GetVersionInfo() */
#define CANSM_SID_GETVERSIONINFO 0x01U

#if (defined CANSM_SID_REQUESTCOMMODE)      /* To prevent double declaration */
#error CANSM_SID_REQUESTCOMMODE already defined!
#endif /* #if (defined CANSM_SID_REQUESTCOMMODE) */

/** \brief Service Id of CanSM_RequestComMode() */
#define CANSM_SID_REQUESTCOMMODE 0x02U

#if (defined CANSM_SID_GETCURRENTCOMMODE)      /* To prevent double declaration */
#error CANSM_SID_GETCURRENTCOMMODE already defined!
#endif /* #if (defined CANSM_SID_GETCURRENTCOMMODE) */

/** \brief Service Id of CanSM_GetCurrentComMode() */
#define CANSM_SID_GETCURRENTCOMMODE 0x03U

#if (defined CANSM_SID_CONTROLLERBUSOFF)      /* To prevent double declaration */
#error CANSM_SID_CONTROLLERBUSOFF already defined!
#endif /* #if (defined CANSM_SID_CONTROLLERBUSOFF) */

/** \brief Service Id of CanSM_ControllerBusOff() */
#define CANSM_SID_CONTROLLERBUSOFF 0x04U

#if (defined CANSM_SID_MAINFUNCTION)      /* To prevent double declaration */
#error CANSM_SID_MAINFUNCTION already defined!
#endif /* #if (defined CANSM_SID_MAINFUNCTION) */

/** \brief Service Id of CanSM_MainFunction() */
#define CANSM_SID_MAINFUNCTION 0x05U

#if (defined CANSM_SID_CONTROLLERMODEINDICATION)      /* To prevent double declaration */
#error CANSM_SID_CONTROLLERMODEINDICATION already defined!
#endif /* #if (defined CANSM_SID_CONTROLLERMODEINDICATION) */

/** \brief Service Id of CanSM_ControllerModeIndication() */
#define CANSM_SID_CONTROLLERMODEINDICATION 0x07U

#if (defined CANSM_SID_TRANSCEIVERMODEINDICATION)      /* To prevent double declaration */
#error CANSM_SID_TRANSCEIVERMODEINDICATION already defined!
#endif /* #if (defined CANSM_SID_TRANSCEIVERMODEINDICATION) */

/** \brief Service Id of CanSM_TransceiverModeIndication() */
#define CANSM_SID_TRANSCEIVERMODEINDICATION 0x09U

#if (defined CANSM_SID_CLEARTRCVWUFFLAGINDICATION)      /* To prevent double declaration */
#error CANSM_SID_CLEARTRCVWUFFLAGINDICATION already defined!
#endif /* #if (defined CANSM_SID_CLEARTRCVWUFFLAGINDICATION) */

/** \brief Service Id of CanSM_ClearTrcvWufFlagIndication() */
#define CANSM_SID_CLEARTRCVWUFFLAGINDICATION 0x08U

#if (defined CANSM_SID_CHECKTRANSCEIVERWAKEFLAGINDICATION)      /* To prevent double declaration */
#error CANSM_SID_CHECKTRANSCEIVERWAKEFLAGINDICATION already defined!
#endif /* #if (defined CANSM_SID_CHECKTRANSCEIVERWAKEFLAGINDICATION) */

/** \brief Service Id of CanSM_CheckTransceiverWakeFlagIndication() */
#define CANSM_SID_CHECKTRANSCEIVERWAKEFLAGINDICATION 0x0aU

#if (defined CANSM_SID_CONFIRMPNAVAILABILITY)      /* To prevent double declaration */
#error CANSM_SID_CONFIRMPNAVAILABILITY already defined!
#endif /* #if (defined CANSM_SID_CONFIRMPNAVAILABILITY) */

/** \brief Service Id of CanSM_ConfirmPnAvailability() */
#define CANSM_SID_CONFIRMPNAVAILABILITY 0x06U

#if (defined CANSM_SID_TXTIMEOUTEXCEPTION)      /* To prevent double declaration */
#error CANSM_SID_TXTIMEOUTEXCEPTION already defined!
#endif /* #if (defined CANSM_SID_TXTIMEOUTEXCEPTION) */

/** \brief Service Id of CanSM_TxTimeoutException() */
#define CANSM_SID_TXTIMEOUTEXCEPTION 0x0bU

#if (defined CANSM_SID_SETBAUDRATE)      /* To prevent double declaration */
#error CANSM_SID_SETBAUDRATE already defined!
#endif /* #if (defined CANSM_SID_SETBAUDRATE) */

/** \brief Service Id of CanSM_SetBaudrate() */
#define CANSM_SID_SETBAUDRATE 0x0dU

/*------------------[Error Ids]-----------*/

#if (defined CANSM_E_UNINIT)      /* To prevent double declaration */
#error CANSM_E_UNINIT already defined!
#endif /* #if (defined CANSM_E_UNINIT) */

/** \brief DET Error Code */
#define CANSM_E_UNINIT 0x01U

#if (defined CANSM_E_PARAM_POINTER)      /* To prevent double declaration */
#error CANSM_E_PARAM_POINTER already defined!
#endif /* #if (defined CANSM_E_PARAM_POINTER) */

/** \brief DET Error Code */
#define CANSM_E_PARAM_POINTER 0x02U

#if (defined CANSM_E_INVALID_NETWORK_HANDLE)      /* To prevent double declaration */
#error CANSM_E_INVALID_NETWORK_HANDLE already defined!
#endif /* #if (defined CANSM_E_INVALID_NETWORK_HANDLE) */

/** \brief DET Error Code */
#define CANSM_E_INVALID_NETWORK_HANDLE 0x03U

#if (defined CANSM_E_PARAM_CONTROLLER)      /* To prevent double declaration */
#error CANSM_E_PARAM_CONTROLLER already defined!
#endif /* #if (defined CANSM_E_PARAM_CONTROLLER) */

/** \brief DET Error Code */
#define CANSM_E_PARAM_CONTROLLER 0x04U

#if (defined CANSM_E_PARAM_TRANSCEIVER)      /* To prevent double declaration */
#error CANSM_E_PARAM_TRANSCEIVER already defined!
#endif /* #if (defined CANSM_E_PARAM_TRANSCEIVER) */

/** \brief DET Error Code */
#define CANSM_E_PARAM_TRANSCEIVER 0x05U

#if (defined CANSM_E_INVALID_COMM_REQUEST)      /* To prevent double declaration */
#error CANSM_E_INVALID_COMM_REQUEST already defined!
#endif /* #if (defined CANSM_E_INVALID_COMM_REQUEST) */

/** \brief DET Error Code */
#define CANSM_E_INVALID_COMM_REQUEST 0x08U

#if (defined CANSM_E_MODE_REQUEST_TIMEOUT)      /* To prevent double declaration */
#error CANSM_E_MODE_REQUEST_TIMEOUT already defined!
#endif /* #if (defined CANSM_E_MODE_REQUEST_TIMEOUT) */

/** \brief DET Error Code: timeout waiting for mode change indication */
#define CANSM_E_MODE_REQUEST_TIMEOUT 0x0AU

#if (defined CANSM_E_TXTIMEOUT_RECOVERY_ACTIVE)      /* To prevent double declaration */
#error CANSM_E_TXTIMEOUT_RECOVERY_ACTIVE already defined!
#endif /* #if (defined CANSM_E_TXTIMEOUT_RECOVERY_ACTIVE) */

/** \brief DET Error Code: Denied mode req. due to ongoing Tx timeout recovery (EB enhancement) */
#define CANSM_E_TXTIMEOUT_RECOVERY_ACTIVE 0x0CU


/*------------------[CanSM states]-----------*/

#if (defined CANSM_S_NOIN)      /* To prevent double declaration */
#error CANSM_S_NOIN already defined!
#endif /* #if (defined CANSM_S_NOIN) */

/** \brief Value of state "NOIN" */
#define CANSM_S_NOIN                      0x00U

#if (defined CANSM_S_NOCO)      /* To prevent double declaration */
#error CANSM_S_NOCO already defined!
#endif /* #if (defined CANSM_S_NOCO) */

/** \brief Value of state "NOCO" */
#define CANSM_S_NOCO                      0x10U

#if (defined CANSM_S_SICO)      /* To prevent double declaration */
#error CANSM_S_SICO already defined!
#endif /* #if (defined CANSM_S_SICO) */

/** \brief Value of state "SICO" */
#define CANSM_S_SICO                      0x20U

#if (defined CANSM_S_FUCO)      /* To prevent double declaration */
#error CANSM_S_FUCO already defined!
#endif /* #if (defined CANSM_S_FUCO) */

/** \brief Value of the upper nibble for all substates of main state "FUCO" */
#define CANSM_S_FUCO                      0x30U

#if (defined CANSM_S_FUCO_BUS_OFF_CHECK)      /* To prevent double declaration */
#error CANSM_S_FUCO_BUS_OFF_CHECK already defined!
#endif /* #if (defined CANSM_S_FUCO_BUS_OFF_CHECK) */

/** \brief Value of state "FUCO_BUS_OFF_CHECK" (substate of FUCO) */
#define CANSM_S_FUCO_BUS_OFF_CHECK      ( CANSM_S_FUCO | 0x00U )

#if (defined CANSM_S_FUCO_NO_BUS_OFF)      /* To prevent double declaration */
#error CANSM_S_FUCO_NO_BUS_OFF already defined!
#endif /* #if (defined CANSM_S_FUCO_NO_BUS_OFF) */

/** \brief Value of state "FUCO_NO_BUS_OFF" (substate of FUCO) */
#define CANSM_S_FUCO_NO_BUS_OFF         ( CANSM_S_FUCO | 0x01U )

#if (defined CANSM_S_FUCO_RESTART_CC)      /* To prevent double declaration */
#error CANSM_S_FUCO_RESTART_CC already defined!
#endif /* #if (defined CANSM_S_FUCO_RESTART_CC) */

/** \brief Value of state "FUCO_RESTART_CC" (substate of FUCO) */
#define CANSM_S_FUCO_RESTART_CC         ( CANSM_S_FUCO | 0x02U )

#if (defined CANSM_S_FUCO_TX_OFF)      /* To prevent double declaration */
#error CANSM_S_FUCO_TX_OFF already defined!
#endif /* #if (defined CANSM_S_FUCO_TX_OFF) */

/** \brief Value of state "FUCO_TX_OFF" (substate of FUCO) */
#define CANSM_S_FUCO_TX_OFF             ( CANSM_S_FUCO | 0x03U )

#if (defined CANSM_S_FUCO_OFF)      /* To prevent double declaration */
#error CANSM_S_FUCO_OFF already defined!
#endif /* #if (defined CANSM_S_FUCO_OFF) */

#define CANSM_S_FUCO_OFF             ( CANSM_S_FUCO | 0x04U )

#if (defined CANSM_S_FUCO_BUS_DEACTIVATED_RECOVERY)    /* To prevent double declaration */
#error CANSM_S_FUCO_BUS_DEACTIVATED_RECOVERY already defined!
#endif /* #if (CANSM_S_FUCO_BUS_DEACTIVATED_RECOVERY) */

#define CANSM_S_FUCO_BUS_DEACTIVATED_RECOVERY  ( CANSM_S_FUCO | 0x05U )

#if (defined CANSM_S_RNOCO)      /* To prevent double declaration */
#error CANSM_S_RNOCO already defined!
#endif /* #if (defined CANSM_S_RNOCO) */

/** \brief Value of the upper nibble for all substates of main state "RNOCO" */
#define CANSM_S_RNOCO                     0x40U

#if (defined CANSM_S_RNOCO_CAN_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_S_RNOCO_CAN_CC_STOPPED already defined!
#endif /* #if (defined CANSM_S_RNOCO_CAN_CC_STOPPED) */

/** \brief Value of state "RNOCO_CAN_CC_STOPPED" (substate of RNOCO) */
#define CANSM_S_RNOCO_CAN_CC_STOPPED    ( CANSM_S_RNOCO | 0x00U )

#if (defined CANSM_S_RNOCO_CAN_CC_SLEEP)      /* To prevent double declaration */
#error CANSM_S_RNOCO_CAN_CC_SLEEP already defined!
#endif /* #if (defined CANSM_S_RNOCO_CAN_CC_SLEEP) */

/** \brief Value of state "RNOCO_CAN_CC_SLEEP" (substate of RNOCO) */
#define CANSM_S_RNOCO_CAN_CC_SLEEP      ( CANSM_S_RNOCO | 0x01U )

#if (defined CANSM_S_RNOCO_CAN_TRCV_NORMAL)      /* To prevent double declaration */
#error CANSM_S_RNOCO_CAN_TRCV_NORMAL already defined!
#endif /* #if (defined CANSM_S_RNOCO_CAN_TRCV_NORMAL) */

/** \brief Value of state "RNOCO_CAN_TRCV_NORMAL" (substate of RNOCO) */
#define CANSM_S_RNOCO_CAN_TRCV_NORMAL   ( CANSM_S_RNOCO | 0x02U )

#if (defined CANSM_S_RNOCO_CAN_TRCV_STANDBY)      /* To prevent double declaration */
#error CANSM_S_RNOCO_CAN_TRCV_STANDBY already defined!
#endif /* #if (defined CANSM_S_RNOCO_CAN_TRCV_STANDBY) */

/** \brief Value of state "RNOCO_CAN_TRCV_STANDBY" (substate of RNOCO) */
#define CANSM_S_RNOCO_CAN_TRCV_STANDBY  ( CANSM_S_RNOCO | 0x03U )

#if (defined CANSM_S_RFUCO)      /* To prevent double declaration */
#error CANSM_S_RFUCO already defined!
#endif /* #if (defined CANSM_S_RFUCO) */

/** \brief Value of the upper nibble for all substates of main state "RFUCO" */
#define CANSM_S_RFUCO                     0x50U

#if (defined CANSM_S_RFUCO_CAN_TRCV_NORMAL)      /* To prevent double declaration */
#error CANSM_S_RFUCO_CAN_TRCV_NORMAL already defined!
#endif /* #if (defined CANSM_S_RFUCO_CAN_TRCV_NORMAL) */

/** \brief Value of state "RFUCO_CAN_TRCV_NORMAL" (substate of RFUCO) */
#define CANSM_S_RFUCO_CAN_TRCV_NORMAL   ( CANSM_S_RFUCO | 0x00U )

#if (defined CANSM_S_RFUCO_CAN_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_S_RFUCO_CAN_CC_STOPPED already defined!
#endif /* #if (defined CANSM_S_RFUCO_CAN_CC_STOPPED) */

/** \brief Value of state "RFUCO_CAN_CC_STOPPED" (substate of RFUCO) */
#define CANSM_S_RFUCO_CAN_CC_STOPPED    ( CANSM_S_RFUCO | 0x01U )

#if (defined CANSM_S_RFUCO_CAN_CC_STARTED)      /* To prevent double declaration */
#error CANSM_S_RFUCO_CAN_CC_STARTED already defined!
#endif /* #if (defined CANSM_S_RFUCO_CAN_CC_STARTED) */

/** \brief Value of state "RFUCO_CAN_CC_STARTED" (substate of RFUCO) */
#define CANSM_S_RFUCO_CAN_CC_STARTED    ( CANSM_S_RFUCO | 0x02U )

#if (defined CANSM_S_PN)      /* To prevent double declaration */
#error CANSM_S_PN already defined!
#endif /* #if (defined CANSM_S_PN) */

/** \brief Value of the upper nibble for all substates of main state "PN" */
#define CANSM_S_PN                                0x60U

#if (defined CANSM_S_PN_CLEAR_WUF)      /* To prevent double declaration */
#error CANSM_S_PN_CLEAR_WUF already defined!
#endif /* #if (defined CANSM_S_PN_CLEAR_WUF) */

/** \brief Value of state "PN_CLEAR_WUF" (substate of PN) */
#define CANSM_S_PN_CLEAR_WUF                    ( CANSM_S_PN | 0x00U )

#if (defined CANSM_S_PN_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_S_PN_CC_STOPPED already defined!
#endif /* #if (defined CANSM_S_PN_CC_STOPPED) */

/** \brief Value of state "PN_CC_STOPPED" (substate of PN) */
#define CANSM_S_PN_CC_STOPPED                   ( CANSM_S_PN | 0x01U )

#if (defined CANSM_S_PN_TRCV_NORMAL)      /* To prevent double declaration */
#error CANSM_S_PN_TRCV_NORMAL already defined!
#endif /* #if (defined CANSM_S_PN_TRCV_NORMAL) */

/** \brief Value of state "PN_TRCV_NORMAL" (substate of PN) */
#define CANSM_S_PN_TRCV_NORMAL                  ( CANSM_S_PN | 0x02U )

#if (defined CANSM_S_PN_TRCV_STANDBY)      /* To prevent double declaration */
#error CANSM_S_PN_TRCV_STANDBY already defined!
#endif /* #if (defined CANSM_S_PN_TRCV_STANDBY) */

/** \brief Value of state "PN_TRCV_STANDBY" (substate of PN) */
#define CANSM_S_PN_TRCV_STANDBY                 ( CANSM_S_PN | 0x03U )

#if (defined CANSM_S_PN_CC_SLEEP)      /* To prevent double declaration */
#error CANSM_S_PN_CC_SLEEP already defined!
#endif /* #if (defined CANSM_S_PN_CC_SLEEP) */

/** \brief Value of state "PN_CC_SLEEP" (substate of PN) */
#define CANSM_S_PN_CC_SLEEP                     ( CANSM_S_PN | 0x04U )

#if (defined CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP)      /* To prevent double declaration */
#error CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP already defined!
#endif /* #if (defined CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP) */

/** \brief Value of state "PN_CHECK_WFLAG_IN_CC_SLEEP" (substate of PN) */
#define CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP      ( CANSM_S_PN | 0x05U )

#if (defined CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP)      /* To prevent double declaration */
#error CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP already defined!
#endif /* #if (defined CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP) */

/** \brief Value of state "PN_CHECK_WFLAG_IN_NOT_CC_SLEEP" (substate of PN) */
#define CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP  ( CANSM_S_PN | 0x06U )

#if (defined CANSM_S_CHBR)      /* To prevent double declaration */
#error CANSM_S_CHBR already defined!
#endif /* #if (defined CANSM_S_CHBR) */

/** \brief Value of the upper nibble for all substates of main state "CHBR" */
#define CANSM_S_CHBR                      0x70U

#if (defined CANSM_S_CHBR_CAN_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_S_CHBR_CAN_CC_STOPPED already defined!
#endif /* #if (defined CANSM_S_CHBR_CAN_CC_STOPPED) */

/** \brief Value of state "CHBR_CAN_CC_STOPPED" (substate of CHBR) */
#define CANSM_S_CHBR_CAN_CC_STOPPED     ( CANSM_S_CHBR | 0x00U )

#if (defined CANSM_S_CHBR_CAN_CC_STARTED)      /* To prevent double declaration */
#error CANSM_S_CHBR_CAN_CC_STARTED already defined!
#endif /* #if (defined CANSM_S_CHBR_CAN_CC_STARTED) */

/** \brief Value of state "CHBR_CAN_CC_STARTED" (substate of CHBR) */
#define CANSM_S_CHBR_CAN_CC_STARTED     ( CANSM_S_CHBR | 0x01U )

#if (defined CANSM_S_TX_TIMEOUT)      /* To prevent double declaration */
#error CANSM_S_TX_TIMEOUT already defined!
#endif /* #if (defined CANSM_S_TX_TIMEOUT) */

/** \brief Value of the upper nibble for all substates of main state "TX_TIMEOUT" */
#define CANSM_S_TX_TIMEOUT                0x80U

#if (defined CANSM_S_TX_TIMEOUT_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_S_TX_TIMEOUT_CC_STOPPED already defined!
#endif /* #if (defined CANSM_S_TX_TIMEOUT_CC_STOPPED) */

/** \brief Value of state "TX_TIMEOUT_CC_STOPPED" (substate of TX_TIMEOUT) */
#define CANSM_S_TX_TIMEOUT_CC_STOPPED   ( CANSM_S_TX_TIMEOUT | 0x00U )

#if (defined CANSM_S_TX_TIMEOUT_CC_STARTED)      /* To prevent double declaration */
#error CANSM_S_TX_TIMEOUT_CC_STARTED already defined!
#endif /* #if (defined CANSM_S_TX_TIMEOUT_CC_STARTED) */

/** \brief Value of state "TX_TIMEOUT_CC_STARTED" (substate of TX_TIMEOUT) */
#define CANSM_S_TX_TIMEOUT_CC_STARTED   ( CANSM_S_TX_TIMEOUT | 0x01U )


#if (defined CANSM_GET_MAIN_STATE)      /* To prevent double declaration */
#error CANSM_GET_MAIN_STATE already defined!
#endif /* #if (defined CANSM_GET_MAIN_STATE) */

/** \brief Gets the upper nibble from a state value to identify the corresponding main state
 **
 ** \param[in] state Value of the internal state
 */
#define CANSM_GET_MAIN_STATE(state) ( (state) & 0xf0U )

/*------------------[CanSM transitions]-----------*/

#if (defined CANSM_T_NONE)      /* To prevent double declaration */
#error CANSM_T_NONE already defined!
#endif /* #if (defined CANSM_T_NONE) */

/** \brief Value for no transition */
#define CANSM_T_NONE                   0x00U

#if (defined CANSM_T_SICO_FUCO)      /* To prevent double declaration */
#error CANSM_T_SICO_FUCO already defined!
#endif /* #if (defined CANSM_T_SICO_FUCO) */

/** \brief Value for transition from SICO to FUCO according to ASR40 SWS Figure 7-1 */
#define CANSM_T_SICO_FUCO              0x10U

#if (defined CANSM_T_FUCO)      /* To prevent double declaration */
#error CANSM_T_FUCO already defined!
#endif /* #if (defined CANSM_T_FUCO) */

/** \brief Value of upper nibble for all FUCO transitions according to ASR40 SWS Figure 7-6 */
#define CANSM_T_FUCO                   0x30U

#if (defined CANSM_T_FUCO_SICO)      /* To prevent double declaration */
#error CANSM_T_FUCO_SICO already defined!
#endif /* #if (defined CANSM_T_FUCO_SICO) */

/** \brief Value for transition from CANSM_S_FUCO_BUS_OFF_CHECK and
 ** CANSM_S_FUCO_NO_BUS_OFF to CANSM_S_SICO */
#define CANSM_T_FUCO_SICO            ( CANSM_T_FUCO | 0x00U )

#if (defined CANSM_T_FUCO_NOCO)      /* To prevent double declaration */
#error CANSM_T_FUCO_NOCO already defined!
#endif /* #if (defined CANSM_T_FUCO_NOCO) */

/** \brief Value for transition from CANSM_S_FUCO to CANSM_S_NOCO */
#define CANSM_T_FUCO_NOCO            ( CANSM_T_FUCO | 0x01U )

#if (defined CANSM_T_FUCO_CHBR)      /* To prevent double declaration */
#error CANSM_T_FUCO_CHBR already defined!
#endif /* #if (defined CANSM_T_FUCO_CHBR) */

/** \brief Value for transition from FUCO to CHBR */
#define CANSM_T_FUCO_CHBR            ( CANSM_T_FUCO | 0x02U )

#if (defined CANSM_T_FUCO_HANDLE_BUS_OFF)      /* To prevent double declaration */
#error CANSM_T_FUCO_HANDLE_BUS_OFF already defined!
#endif /* #if (defined CANSM_T_FUCO_HANDLE_BUS_OFF) */

/** \brief All transitions to CANSM_S_FUCO_RESTART_CC */
#define CANSM_T_FUCO_HANDLE_BUS_OFF  ( CANSM_T_FUCO | 0x03U )

#if (defined CANSM_T_FUCO_TX_OFF)      /* To prevent double declaration */
#error CANSM_T_FUCO_TX_OFF already defined!
#endif /* #if (defined CANSM_T_FUCO_TX_OFF) */

/** \brief Value for transition according to ASR40 SWS Figure 7-4 */
#define CANSM_T_FUCO_TX_OFF          ( CANSM_T_FUCO | 0x04U )

#if (defined CANSM_T_FUCO_TX_ON)      /* To prevent double declaration */
#error CANSM_T_FUCO_TX_ON already defined!
#endif /* #if (defined CANSM_T_FUCO_TX_ON) */

/** \brief Value for transition according to ASR40 SWS Figure 7-4 */
#define CANSM_T_FUCO_TX_ON           ( CANSM_T_FUCO | 0x05U )

#if (defined CANSM_T_FUCO_BUS_OFF_PASSIVE)      /* To prevent double declaration */
#error CANSM_T_FUCO_BUS_OFF_PASSIVE already defined!
#endif /* #if (defined CANSM_T_FUCO_BUS_OFF_PASSIVE) */

/** \brief Value for transition according to ASR40 SWS Figure 7-4 */
#define CANSM_T_FUCO_BUS_OFF_PASSIVE ( CANSM_T_FUCO | 0x06U )

#if (defined CANSM_T_FUCO_OFF)      /* To prevent double declaration */
#error CANSM_T_FUCO_OFF already defined!
#endif /* #if (defined CANSM_T_FUCO_OFF) */

#define CANSM_T_FUCO_OFF           ( CANSM_T_FUCO | 0x07U )

#if (defined CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY)      /* To prevent double declaration */
#error CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY already defined!
#endif /* #if (defined CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY) */

#define CANSM_T_FUCO_BUS_DEACTIVATED_RECOVERY  ( CANSM_T_FUCO | 0x08U )

#if (defined CANSM_T_RNOCO)      /* To prevent double declaration */
#error CANSM_T_RNOCO already defined!
#endif /* #if (defined CANSM_T_RNOCO) */

/** \brief Value of upper nibble for all RNOCO transitions according to ASR40 SWS Figure 7-4 */
#define CANSM_T_RNOCO                  0x40U

#if (defined CANSM_T_RNOCO_INITIAL)      /* To prevent double declaration */
#error CANSM_T_RNOCO_INITIAL already defined!
#endif /* #if (defined CANSM_T_RNOCO_INITIAL) */

/** \brief Value for initial transition to RNOCO (E_PRE_NO_COM) */
#define CANSM_T_RNOCO_INITIAL        ( CANSM_T_RNOCO | 0x00U )

#if (defined CANSM_T_RNOCO_DEINIT_INITIAL)      /* To prevent double declaration */
#error CANSM_T_RNOCO_DEINIT_INITIAL already defined!
#endif /* #if (defined CANSM_T_RNOCO_DEINIT_INITIAL) */

/** \brief Value for transition for the choice between Partial Network on/off */
#define CANSM_T_RNOCO_DEINIT_INITIAL ( CANSM_T_RNOCO | 0x01U )

#if (defined CANSM_T_RNOCO_CC_INITIAL)      /* To prevent double declaration */
#error CANSM_T_RNOCO_CC_INITIAL already defined!
#endif /* #if (defined CANSM_T_RNOCO_CC_INITIAL) */

/** \brief Value for transition to CANSM_S_RNOCO_CAN_CC_STOPPED */
#define CANSM_T_RNOCO_CC_INITIAL     ( CANSM_T_RNOCO | 0x02U )

#if (defined CANSM_T_RNOCO_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_T_RNOCO_CC_STOPPED already defined!
#endif /* #if (defined CANSM_T_RNOCO_CC_STOPPED) */

/** \brief Value for transition according to ASR40 SWS rev 3 Figure 7-4 */
#define CANSM_T_RNOCO_CC_STOPPED     ( CANSM_T_RNOCO | 0x03U )

#if (defined CANSM_T_RNOCO_CC_SLEEP)      /* To prevent double declaration */
#error CANSM_T_RNOCO_CC_SLEEP already defined!
#endif /* #if (defined CANSM_T_RNOCO_CC_SLEEP) */

/** \brief Value for transition according to ASR40 SWS rev 3 Figure 7-4 */
#define CANSM_T_RNOCO_CC_SLEEP       ( CANSM_T_RNOCO | 0x04U )

#if (defined CANSM_T_RNOCO_TRCV_NORMAL)      /* To prevent double declaration */
#error CANSM_T_RNOCO_TRCV_NORMAL already defined!
#endif /* #if (defined CANSM_T_RNOCO_TRCV_NORMAL) */

/** \brief Value for transition according to ASR40 SWS rev 3 Figure 7-4 */
#define CANSM_T_RNOCO_TRCV_NORMAL    ( CANSM_T_RNOCO | 0x05U )

#if (defined CANSM_T_RNOCO_FINAL)      /* To prevent double declaration */
#error CANSM_T_RNOCO_FINAL already defined!
#endif /* #if (defined CANSM_T_RNOCO_FINAL) */

/** \brief Value for transition to CANSM_S_NOCO */
#define CANSM_T_RNOCO_FINAL          ( CANSM_T_RNOCO | 0x06U )

#if (defined CANSM_T_RFUCO)      /* To prevent double declaration */
#error CANSM_T_RFUCO already defined!
#endif /* #if (defined CANSM_T_RFUCO) */

/** \brief Value of upper nibble for all RFUCO transitions according to ASR40 SWS Figure 7-5 */
#define CANSM_T_RFUCO                  0x50U

#if (defined CANSM_T_RFUCO_INITIAL)      /* To prevent double declaration */
#error CANSM_T_RFUCO_INITIAL already defined!
#endif /* #if (defined CANSM_T_RFUCO_INITIAL) */

/** \brief Value for initial transition (Branches to CANSM_T_RFUCO_TRCV_INITIAL
 ** (if trcv is present) or CANSM_T_RFUCO_TRCV_NORMAL) */
#define CANSM_T_RFUCO_INITIAL        ( CANSM_T_RFUCO | 0x00U )

#if (defined CANSM_T_RFUCO_TRCV_INITIAL)      /* To prevent double declaration */
#error CANSM_T_RFUCO_TRCV_INITIAL already defined!
#endif /* #if (defined CANSM_T_RFUCO_TRCV_INITIAL) */

/** \brief Value for transition to CANSM_S_RFUCO_CAN_TRCV_NORMAL */
#define CANSM_T_RFUCO_TRCV_INITIAL   ( CANSM_T_RFUCO | 0x01U )

#if (defined CANSM_T_RFUCO_TRCV_NORMAL)      /* To prevent double declaration */
#error CANSM_T_RFUCO_TRCV_NORMAL already defined!
#endif /* #if (defined CANSM_T_RFUCO_TRCV_NORMAL) */

/** \brief Value for transition according to ASR40 SWS Figure 7-3 */
#define CANSM_T_RFUCO_TRCV_NORMAL    ( CANSM_T_RFUCO | 0x02U )

#if (defined CANSM_T_RFUCO_CC_STOPPED)      /* To prevent double declaration */
#error CANSM_T_RFUCO_CC_STOPPED already defined!
#endif /* #if (defined CANSM_T_RFUCO_CC_STOPPED) */

/** \brief Value for transition according to ASR40 SWS Figure 7-3 */
#define CANSM_T_RFUCO_CC_STOPPED     ( CANSM_T_RFUCO | 0x03U )

#if (defined CANSM_T_RFUCO_CC_STARTED)      /* To prevent double declaration */
#error CANSM_T_RFUCO_CC_STARTED already defined!
#endif /* #if (defined CANSM_T_RFUCO_CC_STARTED) */

/** \brief Value for transition according to ASR40 SWS Figure 7-3 */
#define CANSM_T_RFUCO_CC_STARTED     ( CANSM_T_RFUCO | 0x04U )

#if (defined CANSM_T_PN)      /* To prevent double declaration */
#error CANSM_T_PN already defined!
#endif /* #if (defined CANSM_T_PN) */

/** \brief Value of upper nibble for all PN transitions according to ASR40 SWS Figure 7-3 */
#define CANSM_T_PN                          0x60U

#if (defined CANSM_T_PN_DEINIT)      /* To prevent double declaration */
#error CANSM_T_PN_DEINIT already defined!
#endif /* #if (defined CANSM_T_PN_DEINIT) */

/** \brief Value for transition to CANSM_S_PN_CLEAR_WUF */
#define CANSM_T_PN_DEINIT                 ( CANSM_T_PN | 0x00U )

#if (defined CANSM_T_PN_CLEAR_WUF_INDICATED)      /* To prevent double declaration */
#error CANSM_T_PN_CLEAR_WUF_INDICATED already defined!
#endif /* #if (defined CANSM_T_PN_CLEAR_WUF_INDICATED) */

/** \brief Value for transition to CANSM_S_PN_CC_STOPPED */
#define CANSM_T_PN_CLEAR_WUF_INDICATED    ( CANSM_T_PN | 0x01U )

#if (defined CANSM_T_PN_CC_STOPPED_INDICATED)      /* To prevent double declaration */
#error CANSM_T_PN_CC_STOPPED_INDICATED already defined!
#endif /* #if (defined CANSM_T_PN_CC_STOPPED_INDICATED) */

/** \brief Value for transition to CANSM_S_PN_TRCV_NORMAL */
#define CANSM_T_PN_CC_STOPPED_INDICATED   ( CANSM_T_PN | 0x02U )

#if (defined CANSM_T_PN_TRCV_NORMAL_INDICATED)      /* To prevent double declaration */
#error CANSM_T_PN_TRCV_NORMAL_INDICATED already defined!
#endif /* #if (defined CANSM_T_PN_TRCV_NORMAL_INDICATED) */

/** \brief Value for transition to CANSM_S_PN_TRCV_STANDBY */
#define CANSM_T_PN_TRCV_NORMAL_INDICATED  ( CANSM_T_PN | 0x03U )

#if (defined CANSM_T_PN_TRCV_STANDBY_INDICATED)      /* To prevent double declaration */
#error CANSM_T_PN_TRCV_STANDBY_INDICATED already defined!
#endif /* #if (defined CANSM_T_PN_TRCV_STANDBY_INDICATED) */

/** \brief Value for transition to CANSM_S_PN_CC_SLEEP */
#define CANSM_T_PN_TRCV_STANDBY_INDICATED ( CANSM_T_PN | 0x04U )

#if (defined CANSM_T_PN_CC_SLEEP_INDICATED)      /* To prevent double declaration */
#error CANSM_T_PN_CC_SLEEP_INDICATED already defined!
#endif /* #if (defined CANSM_T_PN_CC_SLEEP_INDICATED) */

/** \brief Value for transition to CANSM_S_PN_CHECK_WFLAG_IN_CC_SLEEP */
#define CANSM_T_PN_CC_SLEEP_INDICATED     ( CANSM_T_PN | 0x05U )

#if (defined CANSM_T_PN_CC_SLEEP_TIMEOUT)      /* To prevent double declaration */
#error CANSM_T_PN_CC_SLEEP_TIMEOUT already defined!
#endif /* #if (defined CANSM_T_PN_CC_SLEEP_TIMEOUT) */

/** \brief Value for transition to CANSM_S_PN_CHECK_WFLAG_IN_NOT_CC_SLEEP */
#define CANSM_T_PN_CC_SLEEP_TIMEOUT       ( CANSM_T_PN | 0x06U )

#if (defined CANSM_T_CHBR)      /* To prevent double declaration */
#error CANSM_T_CHBR already defined!
#endif /* #if (defined CANSM_T_CHBR) */

/** \brief Value of upper nibble for all CHBR transitions according to ASR41 SWS */
#define CANSM_T_CHBR                       0x70U

#if (defined CANSM_T_CHBR_CC_STOP)      /* To prevent double declaration */
#error CANSM_T_CHBR_CC_STOP already defined!
#endif /* #if (defined CANSM_T_CHBR_CC_STOP) */

/** \brief Value for transition stopping CC in CHBR */
#define CANSM_T_CHBR_CC_STOP             ( CANSM_T_CHBR | 0x00U )

#if (defined CANSM_T_CHBR_STOPPED_INDICATED)      /* To prevent double declaration */
#error CANSM_T_CHBR_STOPPED_INDICATED already defined!
#endif /* #if (defined CANSM_T_CHBR_STOPPED_INDICATED) */

/** \brief Value for transition from S_CC_STOPPED to S_CC_STARTED */
#define CANSM_T_CHBR_STOPPED_INDICATED   ( CANSM_T_CHBR | 0x01U )

#if (defined CANSM_T_CHBR_CC_START)      /* To prevent double declaration */
#error CANSM_T_CHBR_CC_START already defined!
#endif /* #if (defined CANSM_T_CHBR_CC_START) */

/** \brief Value for transition starting CC in CHBR */
#define CANSM_T_CHBR_CC_START            ( CANSM_T_CHBR | 0x02U )

#if (defined CANSM_T_CHBR_STARTED_INDICATED)      /* To prevent double declaration */
#error CANSM_T_CHBR_STARTED_INDICATED already defined!
#endif /* #if (defined CANSM_T_CHBR_STARTED_INDICATED) */

/** \brief Value for exit transition from S_CC_STARTED */
#define CANSM_T_CHBR_STARTED_INDICATED   ( CANSM_T_CHBR | 0x03U )

#if (defined CANSM_T_TX_TIMEOUT)      /* To prevent double declaration */
#error CANSM_T_TX_TIMEOUT already defined!
#endif /* #if (defined CANSM_T_TX_TIMEOUT) */

/** \brief Value of upper nibble for all TX_TIMEOUT transitions according to ASR41 SWS rev 3 */
#define CANSM_T_TX_TIMEOUT                         0x80U

#if (defined CANSM_T_TX_TIMEOUT_INITIAL)      /* To prevent double declaration */
#error CANSM_T_TX_TIMEOUT_INITIAL already defined!
#endif /* #if (defined CANSM_T_TX_TIMEOUT_INITIAL) */

/** \brief Value for transition according to ASR41 SWS rev 3 Figure 7-8 */
#define CANSM_T_TX_TIMEOUT_INITIAL               ( CANSM_T_TX_TIMEOUT | 0x00U )

#if (defined CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED)      /* To prevent double declaration */
#error CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED already defined!
#endif /* #if (defined CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED) */

/** \brief Value for transition according to ASR41 SWS rev 3 Figure 7-9 */
#define CANSM_T_TX_TIMEOUT_CC_STOPPED_INDICATED  ( CANSM_T_TX_TIMEOUT | 0x01U )

#if (defined CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED)      /* To prevent double declaration */
#error CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED already defined!
#endif /* #if (defined CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED) */

/** \brief Value for transition according to ASR41 SWS rev 3 Figure 7-9 */
#define CANSM_T_TX_TIMEOUT_CC_STARTED_INDICATED  ( CANSM_T_TX_TIMEOUT | 0x02U )

#if (defined CANSM_GET_MAIN_TRANSITION)      /* To prevent double declaration */
#error CANSM_GET_MAIN_TRANSITION already defined!
#endif /* #if (defined CANSM_GET_MAIN_TRANSITION) */

/** \brief Gets the upper nibble from a transition value to identify the corresponding
 ** main transition
 **
 ** \param[in] state Value of the transition
 */
#define CANSM_GET_MAIN_TRANSITION(transition) ( (transition) & 0xf0U )

/*==================[type definitions]======================================*/

/** \brief type big enough to hold our network indices */
typedef NetworkHandleType CanSM_NetworkIndexType;

/** \brief This type defines the internal state of CanSM and holds one value
 ** defined by the CANSM_S_* symbols */
typedef uint8 CanSM_InternalStateType;

/** \brief This type defines the transition of the CanSM states and holds one value
 ** defined by the CANSM_T_* symbols */
typedef uint8 CanSM_TransitionType;

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

#define CANSM_START_SEC_VAR_INIT_UNSPECIFIED
#include <CanSM_MemMap.h>

/** \brief A pointer to this variable (with constant value COMM_NO_COMMUNICATION) is passed to
 *  ComM_BusSM_ModeIndication */
extern VAR(ComM_ModeType, CANSM_VAR) CanSM_CommNoCommunication;

/** \brief A pointer to this variable (with constant value COMM_SILENT_COMMUNICATION) is passed to
 *  ComM_BusSM_ModeIndication */
extern VAR(ComM_ModeType, CANSM_VAR) CanSM_CommSilentCommunication;

/** \brief A pointer to this variable (with constant value COMM_FULL_COMMUNICATION) is passed to
 *  ComM_BusSM_ModeIndication */
extern VAR(ComM_ModeType, CANSM_VAR) CanSM_CommFullCommunication;

#define CANSM_STOP_SEC_VAR_INIT_UNSPECIFIED
#include <CanSM_MemMap.h>


#define CANSM_START_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanSM_MemMap.h>

/** \brief Global CanSM configuration
 **
 ** This is a pointer to the current CanSM configuration.
 */
extern P2CONST( CanSM_ConfigType, CANSM_VAR, CANSM_APPL_CONST )
  CanSM_ConfigPtr;

#define CANSM_STOP_SEC_VAR_CLEARED_UNSPECIFIED
#include <CanSM_MemMap.h>


#define CANSM_START_SEC_VAR_INIT_8
#include <CanSM_MemMap.h>

/** \brief initialization status (treated boolean) */
extern VAR(uint8, CANSM_VAR) CanSM_GlobalInit;

#define CANSM_STOP_SEC_VAR_INIT_8
#include <CanSM_MemMap.h>

/*==================[external function declarations]========================*/

#endif /* ifndef CANSM_INT_STC_H */
/*==================[end of file]===========================================*/

