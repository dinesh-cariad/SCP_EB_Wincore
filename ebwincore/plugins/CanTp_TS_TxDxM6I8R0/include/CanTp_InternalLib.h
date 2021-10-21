/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CANTP_INTERNALLIB_H
#define CANTP_INTERNALLIB_H

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>    /* typedefs for AUTOSAR com stack */
#include <CanTp_Version.h>     /* CanTp version declarations */
#include <CanTp_Cfg.h>         /* DET configuration */
#include <CanTp_Types.h>       /* Module internal global types */
#include <TSAtomic_Assign.h>   /* Atomic assign macros */

#if(CANTP_DEV_ERROR_DETECT == STD_ON)
#include <Det.h>
#endif

/*==================[macros]=================================================*/

#if (defined CANTP_FC_FRAME_NOT_USED) /* to prevent double declaration */
#error CANTP_FC_FRAME_NOT_USED is already defined
#endif /* if (defined CANTP_FC_FRAME_NOT_USED) */

/** \brief Macro to indicate that no FC Tx PDU is used */
#define CANTP_FC_FRAME_NOT_USED  ((PduIdType)0xFFFFU)

#if (defined CANTP_12BIT_MAX_MESSAGE_LENGTH) /* to prevent double declaration */
#error CANTP_12BIT_MAX_MESSAGE_LENGTH is already defined
#endif /* if (defined CANTP_12BIT_MAX_MESSAGE_LENGTH) */

/** \brief Maximal length of segmented CAN message (12 bit). */
#define CANTP_12BIT_MAX_MESSAGE_LENGTH 4095U

/*------------------[list of error codes]------------------------------------*/

#if (defined CANTP_E_PARAM_CONFIG) /* to prevent double declaration */
#error CANTP_E_PARAM_CONFIG is already defined
#endif /* if (defined CANTP_E_PARAM_CONFIG) */
/** \brief no config provided */
#define CANTP_E_PARAM_CONFIG       0x01U

#if (defined CANTP_E_PARAM_ID) /* to prevent double declaration */
#error CANTP_E_PARAM_ID is already defined
#endif /* if (defined CANTP_E_PARAM_ID) */
/** \brief ID parameter not valid */
#define CANTP_E_PARAM_ID           0x02U

#if (defined CANTP_E_PARAM_POINTER) /* to prevent double declaration */
#error CANTP_E_PARAM_POINTER is already defined
#endif /* if (defined CANTP_E_PARAM_POINTER) */
/** \brief API service called with a NULL pointer */
#define CANTP_E_PARAM_POINTER      0x03U

#if (defined CANTP_E_OVERWRITE_FRAME) /* to prevent double declaration */
#error CANTP_E_OVERWRITE_FRAME is already defined
#endif /* if (defined CANTP_E_OVERWRITE_FRAME) */
/** \brief Overwrite previous CF with the next one */
#define CANTP_E_OVERWRITE_FRAME      0x04U

#if (defined CANTP_E_UNINIT) /* to prevent double declaration */
#error CANTP_E_UNINIT is already defined
#endif /* if (defined CANTP_E_UNINIT) */
/** \brief CanTp not initialized */
#define CANTP_E_UNINIT             0x20U

#if (defined CANTP_E_INVALID_TX_ID) /* to prevent double declaration */
#error CANTP_E_INVALID_TX_ID is already defined
#endif /* if (defined CANTP_E_INVALID_TX_ID) */
/** \brief invalid transmit ID */
#define CANTP_E_INVALID_TX_ID      0x30U

#if (defined CANTP_E_INVALID_RX_ID) /* to prevent double declaration */
#error CANTP_E_INVALID_RX_ID is already defined
#endif /* if (defined CANTP_E_INVALID_RX_ID) */
/** \brief invalid receive ID */
#define CANTP_E_INVALID_RX_ID      0x40U

#if (defined CANTP_E_INVALID_TX_LENGTH) /* to prevent double declaration */
#error CANTP_E_INVALID_TX_LENGTH is already defined
#endif /* if (defined CANTP_E_INVALID_TX_LENGTH) */
/** \brief invalid Tx length */
#define CANTP_E_INVALID_TX_LENGTH  0x70U

#if (defined CANTP_E_INVALID_RX_LENGTH) /* to prevent double declaration */
#error CANTP_E_INVALID_RX_LENGTH is already defined
#endif /* if (defined CANTP_E_INVALID_RX_LENGTH) */
/** \brief invalid Rx length */
#define CANTP_E_INVALID_RX_LENGTH  0x80U

#if (defined CANTP_E_INVALID_TATYPE) /* to prevent double declaration */
#error CANTP_E_INVALID_TATYPE is already defined
#endif /* if (defined CANTP_E_INVALID_TATYPE) */
/** \brief invalid communication type */
#define CANTP_E_INVALID_TATYPE     0x90U

#if (defined CANTP_E_PARAM_DIRECTION) /* to prevent double declaration */
#error CANTP_E_PARAM_DIRECTION is already defined
#endif /* if (defined CANTP_E_PARAM_DIRECTION) */
/** \brief invalid direction parameter */
#define CANTP_E_PARAM_DIRECTION    0xA1U

#if (defined CANTP_E_OPER_NOT_SUPPORTED) /* to prevent double declaration */
#error CANTP_E_OPER_NOT_SUPPORTED is already defined
#endif /* if (defined CANTP_E_OPER_NOT_SUPPORTED) */
/** \brief Invalid Requested operation */
#define CANTP_E_OPER_NOT_SUPPORTED 0xA0U

/*------------------[list of AUTOSAR API Ids]--------------------------------*/

#if (defined CANTP_API_UNSPECIFIC) /* to prevent double declaration */
#error CANTP_API_UNSPECIFIC is already defined
#endif /* if (defined CANTP_API_UNSPECIFIC) */
/** \brief service ID for an unspecific API */
#define CANTP_API_UNSPECIFIC               0x00U

#if (defined CANTP_API_INIT) /* to prevent double declaration */
#error CANTP_API_INIT is already defined
#endif /* if (defined CANTP_API_INIT) */
/** \brief service ID for function CanTp_Init() */
#define CANTP_API_INIT                     0x01U

#if (defined CANTP_API_TRANSMIT) /* to prevent double declaration */
#error CANTP_API_TRANSMIT is already defined
#endif /* if (defined CANTP_API_TRANSMIT) */
/** \brief service ID for function CanTp_Transmit() */
#define CANTP_API_TRANSMIT                 0x03U

#if (defined CANTP_API_RXINDICATION) /* to prevent double declaration */
#error CANTP_API_RXINDICATION is already defined
#endif /* if (defined CANTP_API_RXINDICATION) */
/** \brief service ID for function CanTp_RxIndication() */
#define CANTP_API_RXINDICATION             0x42U

#if (defined CANTP_API_TXCONFIRMATION) /* to prevent double declaration */
#error CANTP_API_TXCONFIRMATION is already defined
#endif /* if (defined CANTP_API_TXCONFIRMATION) */
/** \brief service ID for function CanTp_TxConfirmation() */
#define CANTP_API_TXCONFIRMATION           0x40U

#if (defined CANTP_API_MAINFUNCTION) /* to prevent double declaration */
#error CANTP_API_MAINFUNCTION is already defined
#endif /* if (defined CANTP_API_MAINFUNCTION) */
/** \brief service ID for function CanTp_MainFunction() */
#define CANTP_API_MAINFUNCTION             0x06U

#if (defined CANTP_API_GETVERSIONINFO) /* to prevent double declaration */
#error CANTP_API_GETVERSIONINFO is already defined
#endif /* if (defined CANTP_API_GETVERSIONINFO) */
/** \brief service ID for function CanTp_GetVersionInfo() */
#define CANTP_API_GETVERSIONINFO           0x07U

#if (defined CANTP_API_CANCELTRANSMIT) /* to prevent double declaration */
#error CANTP_API_CANCELTRANSMIT is already defined
#endif /* if (defined CANTP_API_CANCELTRANSMIT) */
/** \brief service ID for function CanTp_CancelTransmit() */
#define CANTP_API_CANCELTRANSMIT           0x08U

#if (defined CANTP_API_CANCELRECEIVE) /* to prevent double declaration */
#error CANTP_API_CANCELRECEIVE is already defined
#endif /* if (defined CANTP_API_CANCELRECEIVE) */
/** \brief service ID for function CanTp_CancelReceive() */
#define CANTP_API_CANCELRECEIVE            0x09U

#if (defined CANTP_API_CHANGEPARAMETER) /* to prevent double declaration */
#error CANTP_API_CHANGEPARAMETER is already defined
#endif /* if (defined CANTP_API_CHANGEPARAMETER) */
/** \brief service ID for function CanTp_ChangeParameter() */
#define CANTP_API_CHANGEPARAMETER          0x0AU

#if (defined CANTP_API_CHANGETXPARAMETER) /* to prevent double declaration */
#error CANTP_API_CHANGETXPARAMETER is already defined
#endif /* if (defined CANTP_API_CHANGETXPARAMETER) */
/** \brief service ID for function CanTp_ChangeTxParameter() */
#define CANTP_API_CHANGETXPARAMETER          0x0CU

#if (defined CANTP_API_RESETTXPARAMETER) /* to prevent double declaration */
#error CANTP_API_RESETTXPARAMETER is already defined
#endif /* if (defined CANTP_API_RESETTXPARAMETER) */
/** \brief service ID for function CanTp_ResetTxParameter() */
#define CANTP_API_RESETTXPARAMETER          0x0DU

#if (defined CANTP_API_READPARAMETER) /* to prevent double declaration */
#error CANTP_API_READPARAMETER is already defined
#endif /* if (defined CANTP_API_READPARAMETER) */
/** \brief service ID for function CanTp_ReadParameter() */
#define CANTP_API_READPARAMETER          0x0BU

#if (defined CANTP_API_SETNSA) /* to prevent double declaration */
#error CANTP_API_SETNSA is already defined
#endif /* if (defined CANTP_API_SETNSA) */
/** \brief service ID for function CanTp_SetNSa() */
#define CANTP_API_SETNSA                   0x1EU

#if (defined CANTP_API_GETNSA) /* to prevent double declaration */
#error CANTP_API_GETNSA is already defined
#endif /* if (defined CANTP_API_GETNSA) */
/** \brief service ID for function CanTp_GetNSa() */
#define CANTP_API_GETNSA                   0x1FU

/*------------------[defines signaling alternatives]-------------------------*/

/** \brief defines for global CanTp state machine */
#if (defined CANTP_OFF) /* to prevent double declaration */
#error CANTP_OFF is already defined
#endif /* if (defined CANTP_OFF) */
#define CANTP_OFF   0U     /* = 0 for implementation purposes */

#if (defined CANTP_ON) /* to prevent double declaration */
#error CANTP_ON is already defined
#endif /* if (defined CANTP_ON) */
#define CANTP_ON    1U     /* = 1 for implementation purposes */

#if (defined CANTP_ON_CAN_CAN_FD) /* to prevent double declaration */
#error CANTP_ON_CAN_CAN_FD is already defined
#endif /* if (defined CANTP_ON_CAN_CAN_FD) */
#define CANTP_ON_CAN_CAN_FD    2U     /* = 2 for implementation purposes */

/* Defines for CanTp addressing formats */
#if (defined CANTP_STANDARD) /* to prevent double declaration */
#error CANTP_STANDARD is already defined
#endif /* if (defined CANTP_STANDARD) */
/** \brief Standard addressing format */
#define CANTP_STANDARD  0U

#if (defined CANTP_EXTENDED) /* to prevent double declaration */
#error CANTP_EXTENDED is already defined
#endif /* if (defined CANTP_EXTENDED) */
/** \brief Extended addressing format */
#define CANTP_EXTENDED  1U

#if (defined CANTP_MIXED) /* to prevent double declaration */
#error CANTP_MIXED is already defined
#endif /* if (defined CANTP_MIXED) */
/** \brief Mixed addressing format */
#define CANTP_MIXED     2U

#if (defined CANTP_NORMALFIXED) /* to prevent double declaration */
#error CANTP_NORMALFIXED is already defined
#endif /* if (defined CANTP_MIXED) */
/** \brief Mixed addressing format */
#define CANTP_NORMALFIXED  3U

/** \brief defines for CanTp communication type */
#if (defined CANTP_PHYSICAL) /* to prevent double declaration */
#error CANTP_PHYSICAL is already defined
#endif /* if (defined CANTP_PHYSICAL) */
#define CANTP_PHYSICAL   0U /* physical for 1 to 1 connections */

#if (defined CANTP_FUNCTIONAL) /* to prevent double declaration */
#error CANTP_FUNCTIONAL is already defined
#endif /* if (defined CANTP_FUNCTIONAL) */
#define CANTP_FUNCTIONAL 1U /* function for 1 to n connections */

/** \brief defines for TxNpdu CanIdType */
#if (defined CANTP_TX_STANDARD_CAN) /* to prevent double declaration */
#error CANTP_TX_STANDARD_CAN is already defined
#endif /* if (defined CANTP_TX_STANDARD_CAN) */
#define CANTP_TX_STANDARD_CAN   0U /* CAN 2.0 Tx frame with standard identifier (11bits) */

#if (defined CANTP_TX_EXTENDED_CAN) /* to prevent double declaration */
#error CANTP_TX_EXTENDED_CAN is already defined
#endif /* if (defined CANTP_TX_EXTENDED_CAN) */
#define CANTP_TX_EXTENDED_CAN   1U /* CAN 2.0 Tx frame with extended identifier (29bits) */

#if (defined CANTP_TX_STANDARD_FD_CAN) /* to prevent double declaration */
#error CANTP_TX_STANDARD_FD_CAN is already defined
#endif /* if (defined CANTP_TX_STANDARD_FD_CAN) */
#define CANTP_TX_STANDARD_FD_CAN   2U /* CAN FD Tx frame with standard identifier (11bits) */

#if (defined CANTP_TX_EXTENDED_FD_CAN) /* to prevent double declaration */
#error CANTP_TX_EXTENDED_FD_CAN is already defined
#endif /* if (defined CANTP_TX_EXTENDED_FD_CAN) */
#define CANTP_TX_EXTENDED_FD_CAN   3U /* CAN FD Tx frame with extended identifier (29bits) */

#if (defined CANTP_TX_INVALID_CANID) /* to prevent double declaration */
#error CANTP_TX_INVALID_CANID is already defined
#endif /* if (defined CANTP_TX_INVALID_CANID) */
#define CANTP_TX_INVALID_CANID   255U /* CANFD is not enabled */

/*------------------[NPDU header sizes and NSDU length]---------------------*/

#if (defined CANTP_SF_NPCI_SIZE) /* to prevent double declaration */
#error CANTP_SF_NPCI_SIZE is already defined
#endif /* if (defined CANTP_SF_NPCI_SIZE) */
/** \brief Number of bytes required for SF N_PCI */
#define CANTP_SF_NPCI_SIZE         1U

#if(CANTP_CANFD_ENABLED == STD_ON)

#if (defined CANTP_SF_FD_NPCI_SIZE) /* to prevent double declaration */
#error CANTP_SF_FD_NPCI_SIZE is already defined
#endif /* if (defined CANTP_SF_FD_NPCI_SIZE) */
/** \brief Number of bytes required for SF N_PCI for FD */
#define CANTP_SF_FD_NPCI_SIZE 2U
#endif

#if (defined CANTP_FF_NPCI_SIZE) /* to prevent double declaration */
#error CANTP_FF_NPCI_SIZE is already defined
#endif /* if (defined CANTP_FF_NPCI_SIZE) */
/** \brief Number of bytes required for FF N_PCI */
#define CANTP_FF_NPCI_SIZE         2U

#if (defined CANTP_FF_BIGDATA_NPCI_SIZE) /* to prevent double declaration */
#error CANTP_FF_BIGDATA_NPCI_SIZE is already defined
#endif /* if (defined CANTP_FF_BIGDATA_NPCI_SIZE) */
/** \brief Number of bytes required for FF big data N_PCI */
#define CANTP_FF_BIGDATA_NPCI_SIZE 6U

#if (defined CANTP_CF_NPCI_SIZE) /* to prevent double declaration */
#error CANTP_CF_NPCI_SIZE is already defined
#endif /* if (defined CANTP_CF_NPCI_SIZE) */
/** \brief Number of bytes required for CF N_PCI */
#define CANTP_CF_NPCI_SIZE         1U

#if (defined CANTP_FC_NPCI_SIZE) /* to prevent double declaration */
#error CANTP_FC_NPCI_SIZE is already defined
#endif /* if (defined CANTP_FC_NPCI_SIZE) */
/** \brief Number of bytes required for FC N_PCI */
#define CANTP_FC_NPCI_SIZE         3U

/*------------------[channel state machine]----------------------------------*/

/** \brief defines for CanTp channel state machine
 **
 ** These defines contain all possible channel states for CanTp
 **
 ** Naming: CANTP_CH_<mode>_<action>_<frametype>
 **
 **  <mode>:      RX, TX or IDLE
 **  <action>:    BUFREQ,   poll the upper layer to get a buffer
 **               TRANSMIT, try to transmit using CanIf_Transmit
 **               CONFIRM,  wait for TxConfirmation of transmitted frame
 **               RECEIVE,  wait for RxIndication (answer from remote)
 **               WAITTMR   wait for STmin timeout
 **  <frametype>: SF, FF, CF, FC_CTS, FC_OVFLW, FC_WT
 **
 ** Not all combinations of all substates are possible. The following scheme
 ** is used to pack the states into an uint8 (CanTp_ChStateType):
 ** bits [7|6|5|4|3|2|1|0]
 **      0^mod^act^frame
 ** <mod> 00 idle  <act> 00 BUFREQ    <frame> 000 SF  100 FC_CTS
 **       10 rx          01 TRANSMIT          001 FF  101 FC_OVFLW
 **       11 tx          10 CONFIRM*          010 CF  110 FC_WT
 **                      10 RECEIVE*
 **                      11 WAITTMR
 ** Notes:
 **   * as CONFIRM and RECEIVE can be distinguished by direction and frame
 **     type, they share the same encoding.
 */

/* masks */

#if (defined CANTP_CH_MODE_MASK) /* to prevent double declaration */
#error CANTP_CH_MODE_MASK is already defined
#endif /* if (defined CANTP_CH_MODE_MASK) */
#define CANTP_CH_MODE_MASK        0x60U   /* 0110 0000 */

#if (defined CANTP_CH_ACTION_MASK) /* to prevent double declaration */
#error CANTP_CH_ACTION_MASK is already defined
#endif /* if (defined CANTP_CH_ACTION_MASK) */
/* CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_ACTION_TRANSMIT | CANTP_CH_ACTION_CONFIRM */
#define CANTP_CH_ACTION_MASK      0x98U   /* 1001 1000 */

#if (defined CANTP_CH_FRAME_MASK) /* to prevent double declaration */
#error CANTP_CH_FRAME_MASK is already defined
#endif /* if (defined CANTP_CH_FRAME_MASK) */
#define CANTP_CH_FRAME_MASK       0x07U   /* 0000 0111 */

#if(CANTP_STALLHANDLING_SUPPORT == STD_ON)
#if (defined CANTP_POST_RX_IND_MASK) /* to prevent double declaration */
#error CANTP_POST_RX_IND_MASK is already defined
#endif /* if (defined CANTP_POST_RX_IND_MASK) */
#define CANTP_POST_RX_IND_MASK       0x01U   /* 0000 0001 */
#endif

#if (defined CANTP_POST_TX_CONF_MASK) /* to prevent double declaration */
#error CANTP_POST_TX_CONF_MASK is already defined
#endif /* if (defined CANTP_POST_TX_CONF_MASK) */
#define CANTP_POST_TX_CONF_MASK       0x02U   /* 0000 0010 */

#if (defined CANTP_POST_STMIN_MASK) /* to prevent double declaration */
#error CANTP_POST_STMIN_MASK is already defined
#endif /* if (defined CANTP_POST_STMIN_MASK) */
#define CANTP_POST_STMIN_MASK       0x04U   /* 0000 0100 */

#if (defined CANTP_STMIN_ROUNDUP_MASK) /* to prevent double declaration */
#error CANTP_STMIN_ROUNDUP_MASK is already defined
#endif /* if (defined CANTP_STMIN_ROUNDUP_MASK) */
#define CANTP_STMIN_ROUNDUP_MASK       0x01U   /* 0000 0001 */

#if (defined CANTP_RXCHANN_LOCK_MASK) /* to prevent double declaration */
#error CANTP_RXCHANN_LOCK_MASK is already defined
#endif /* if (defined CANTP_RXCHANN_LOCK_MASK) */
#define CANTP_RXCHANN_LOCK_MASK       0x02U   /* 0000 0010 */

#if (defined CANTP_TXLASTFRAME_MASK) /* to prevent double declaration */
#error CANTP_TXLASTFRAME_MASK is already defined
#endif /* if (defined CANTP_TXLASTFRAME_MASK) */
#define CANTP_TXLASTFRAME_MASK       0x04U   /* 0000 0100 */

#if (defined CANTP_FCWAIT_REPEATED_MASK) /* to prevent double declaration */
#error CANTP_FCWAIT_REPEATED_MASK is already defined
#endif /* if (defined CANTP_FCWAIT_REPEATED_MASK) */
#define CANTP_FCWAIT_REPEATED_MASK       0x08U   /* 0000 1000 */

#if (defined CANTP_FCRECEIVED_MASK) /* to prevent double declaration */
#error CANTP_FCRECEIVED_MASK is already defined
#endif /* if (defined CANTP_FCRECEIVED_MASK) */
#define CANTP_FCRECEIVED_MASK       0x10U   /* 0001 0000 */

/* access macros for substates */

#if (defined CANTP_CH_MODE_IDLE) /* to prevent double declaration */
#error CANTP_CH_MODE_IDLE is already defined
#endif /* if (defined CANTP_CH_MODE_IDLE) */
#define CANTP_CH_MODE_IDLE        0x00U   /* 0000 0000 */

#if (defined CANTP_CH_MODE_RX) /* to prevent double declaration */
#error CANTP_CH_MODE_RX is already defined
#endif /* if (defined CANTP_CH_MODE_RX) */
#define CANTP_CH_MODE_RX          0x40U   /* 0100 0000 */

#if (defined CANTP_CH_MODE_TX) /* to prevent double declaration */
#error CANTP_CH_MODE_TX is already defined
#endif /* if (defined CANTP_CH_MODE_TX) */
#define CANTP_CH_MODE_TX          0x60U   /* 0110 0000 */


#if (defined CANTP_CH_ACTION_BUFREQ) /* to prevent double declaration */
#error CANTP_CH_ACTION_BUFREQ is already defined
#endif /* if (defined CANTP_CH_ACTION_BUFREQ) */
#define CANTP_CH_ACTION_BUFREQ    0x00U   /* 0000 0000 */

#if (defined CANTP_CH_ACTION_TRANSMIT) /* to prevent double declaration */
#error CANTP_CH_ACTION_TRANSMIT is already defined
#endif /* if (defined CANTP_CH_ACTION_TRANSMIT) */
#define CANTP_CH_ACTION_TRANSMIT  0x08U   /* 0000 1000 */

#if (defined CANTP_CH_ACTION_CONFIRM) /* to prevent double declaration */
#error CANTP_CH_ACTION_CONFIRM is already defined
#endif /* if (defined CANTP_CH_ACTION_CONFIRM) */
#define CANTP_CH_ACTION_CONFIRM   0x10U   /* 0001 0000 */

#if (defined CANTP_CH_ACTION_RECEIVE) /* to prevent double declaration */
#error CANTP_CH_ACTION_RECEIVE is already defined
#endif /* if (defined CANTP_CH_ACTION_RECEIVE) */
#define CANTP_CH_ACTION_RECEIVE   0x80U   /* 1000 0000 */

#if (defined CANTP_CH_ACTION_WAITTMR) /* to prevent double declaration */
#error CANTP_CH_ACTION_WAITTMR is already defined
#endif /* if (defined CANTP_CH_ACTION_WAITTMR) */
#define CANTP_CH_ACTION_WAITTMR   0x18U   /* 0001 1000 */


#if (defined CANTP_CH_FRAME_SF) /* to prevent double declaration */
#error CANTP_CH_FRAME_SF is already defined
#endif /* if (defined CANTP_CH_FRAME_SF) */
#define CANTP_CH_FRAME_SF         0x00U   /* 0000 0000 */

#if (defined CANTP_CH_FRAME_FF) /* to prevent double declaration */
#error CANTP_CH_FRAME_FF is already defined
#endif /* if (defined CANTP_CH_FRAME_FF) */
#define CANTP_CH_FRAME_FF         0x01U   /* 0000 0001 */

#if (defined CANTP_CH_FRAME_CF) /* to prevent double declaration */
#error CANTP_CH_FRAME_CF is already defined
#endif /* if (defined CANTP_CH_FRAME_CF) */
#define CANTP_CH_FRAME_CF         0x02U   /* 0000 0010 */

#if (defined CANTP_CH_FRAME_FC_CTS) /* to prevent double declaration */
#error CANTP_CH_FRAME_FC_CTS is already defined
#endif /* if (defined CANTP_CH_FRAME_FC_CTS) */
#define CANTP_CH_FRAME_FC_CTS     0x04U   /* 0000 0100 */

#if (defined CANTP_CH_FRAME_FC_OVFLW) /* to prevent double declaration */
#error CANTP_CH_FRAME_FC_OVFLW is already defined
#endif /* if (defined CANTP_CH_FRAME_FC_OVFLW) */
#define CANTP_CH_FRAME_FC_OVFLW   0x05U   /* 0000 0101 */

#if (defined CANTP_CH_FRAME_FC_WT) /* to prevent double declaration */
#error CANTP_CH_FRAME_FC_WT is already defined
#endif /* if (defined CANTP_CH_FRAME_FC_WT) */
#define CANTP_CH_FRAME_FC_WT      0x06U   /* 0000 0110 */

#if (defined CANTP_CH_ACTION_POSTPONE_FRAME) /* to prevent double declaration */
#error CANTP_CH_ACTION_POSTPONE_FRAME is already defined
#endif /* if (defined CANTP_CH_ACTION_POSTPONE_FRAME) */
#define CANTP_CH_ACTION_POSTPONE_FRAME      0x88U   /* 1000 1000 */

#if (defined CANTP_CH_ACTION_REQUESTED_FCBUFFER) /* to prevent double declaration */
#error CANTP_CH_ACTION_REQUESTED_FCBUFFER is already defined
#endif /* if (defined CANTP_CH_ACTION_REQUESTED_FCBUFFER) */
#define CANTP_CH_ACTION_REQUESTED_FCBUFFER      0x08U   /* 0000 1000 */

/* possible state combinations */

#if (defined CANTP_CH_IDLE) /* to prevent double declaration */
#error CANTP_CH_IDLE is already defined
#endif /* if (defined CANTP_CH_IDLE) */
/** \brief channel is free */
#define CANTP_CH_IDLE             CANTP_CH_MODE_IDLE

#if (defined CANTP_CH_RX_POSTPONED_FC_CTS) /* to prevent double declaration */
#error CANTP_CH_RX_POSTPONED_FC_CTS is already defined
#endif /* if (defined CANTP_CH_RX_POSTPONED_FC_CTS) */
/** \brief rx channel - transmit postpone FC */
#define CANTP_CH_RX_POSTPONED_FC_CTS \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_FRAME_FC_CTS)

#if (defined CANTP_CH_RX_POSTPONED_FC_OVFLW) /* to prevent double declaration */
#error CANTP_CH_RX_POSTPONED_FC_OVFLW is already defined
#endif /* if (defined CANTP_CH_RX_POSTPONED_FC_OVFLW) */
/** \brief rx channel - transmit postpone FC */
#define CANTP_CH_RX_POSTPONED_FC_OVFLW \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_FRAME_FC_OVFLW)

#if (defined CANTP_CH_RX_POSTPONED_FC_WT) /* to prevent double declaration */
#error CANTP_CH_RX_POSTPONED_FC_WT is already defined
#endif /* if (defined CANTP_CH_RX_POSTPONED_FC_WT) */
/** \brief rx channel - transmit postpone FC */
#define CANTP_CH_RX_POSTPONED_FC_WT \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_FRAME_FC_WT)

#if (defined CANTP_CH_RX_BUFREQ_SF) /* to prevent double declaration */
#error CANTP_CH_RX_BUFREQ_SF is already defined
#endif /* if (defined CANTP_CH_RX_BUFREQ_SF) */
/** \brief rx channel - request rx buffer for SF */
#define CANTP_CH_RX_BUFREQ_SF \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_BUFREQ   | CANTP_CH_FRAME_SF)

#if (defined CANTP_CH_RX_BUFREQ_FF) /* to prevent double declaration */
#error CANTP_CH_RX_BUFREQ_FF is already defined
#endif /* if (defined CANTP_CH_RX_BUFREQ_FF) */
/** \brief rx channel - request rx buffer for FF */
#define CANTP_CH_RX_BUFREQ_FF \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_BUFREQ   | CANTP_CH_FRAME_FF)

#if (defined CANTP_CH_RX_BUFREQ_CF) /* to prevent double declaration */
#error CANTP_CH_RX_BUFREQ_CF is already defined
#endif /* if (defined CANTP_CH_RX_BUFREQ_CF) */
/** \brief rx channel - request rx buffer for CF */
#define CANTP_CH_RX_BUFREQ_CF \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_BUFREQ   | CANTP_CH_FRAME_CF)

#if (defined CANTP_CH_RX_TRANSMIT_FC_CTS) /* to prevent double declaration */
#error CANTP_CH_RX_TRANSMIT_FC_CTS is already defined
#endif /* if (defined CANTP_CH_RX_TRANSMIT_FC_CTS) */
/** \brief rx channel - transmit FC next */
#define CANTP_CH_RX_TRANSMIT_FC_CTS \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_TRANSMIT | CANTP_CH_FRAME_FC_CTS)

#if (defined CANTP_CH_RX_TRANSMIT_FC_OVFLW) /* to prevent double declaration */
#error CANTP_CH_RX_TRANSMIT_FC_OVFLW is already defined
#endif /* if (defined CANTP_CH_RX_TRANSMIT_FC_OVFLW) */
/** \brief rx channel - transmit FC next */
#define CANTP_CH_RX_TRANSMIT_FC_OVFLW \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_TRANSMIT | CANTP_CH_FRAME_FC_OVFLW)

#if (defined CANTP_CH_RX_TRANSMIT_FC_WT) /* to prevent double declaration */
#error CANTP_CH_RX_TRANSMIT_FC_WT is already defined
#endif /* if (defined CANTP_CH_RX_TRANSMIT_FC_WT) */
/** \brief rx channel - transmit FC next */
#define CANTP_CH_RX_TRANSMIT_FC_WT \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_TRANSMIT | CANTP_CH_FRAME_FC_WT)

#if (defined CANTP_CH_RX_CONFIRM_FC_CTS) /* to prevent double declaration */
#error CANTP_CH_RX_CONFIRM_FC_CTS is already defined
#endif /* if (defined CANTP_CH_RX_CONFIRM_FC_CTS) */
/** \brief rx channel - wait for FC cts confirmation */
#define CANTP_CH_RX_CONFIRM_FC_CTS \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_CONFIRM  | CANTP_CH_FRAME_FC_CTS)

#if (defined CANTP_CH_RX_CONFIRM_FC_OVFLW) /* to prevent double declaration */
#error CANTP_CH_RX_CONFIRM_FC_OVFLW is already defined
#endif /* if (defined CANTP_CH_RX_CONFIRM_FC_OVFLW) */
/** \brief rx channel - wait for FC ovflw confirmation */
#define CANTP_CH_RX_CONFIRM_FC_OVFLW \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_CONFIRM  | CANTP_CH_FRAME_FC_OVFLW)

#if (defined CANTP_CH_RX_CONFIRM_FC_WT) /* to prevent double declaration */
#error CANTP_CH_RX_CONFIRM_FC_WT is already defined
#endif /* if (defined CANTP_CH_RX_CONFIRM_FC_WT) */
/** \brief rx channel - wait for FC wt confirmation */
#define CANTP_CH_RX_CONFIRM_FC_WT \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_CONFIRM  | CANTP_CH_FRAME_FC_WT)

#if (defined CANTP_CH_RX_RECEIVE_CF) /* to prevent double declaration */
#error CANTP_CH_RX_RECEIVE_CF is already defined
#endif /* if (defined CANTP_CH_RX_RECEIVE_CF) */
/** \brief rx channel - wait for CF reception */
#define CANTP_CH_RX_RECEIVE_CF \
    (uint8)(CANTP_CH_MODE_RX | CANTP_CH_ACTION_RECEIVE  | CANTP_CH_FRAME_CF)

#if (defined CANTP_CH_TX_POSTPONED_SF) /* to prevent double declaration */
#error CANTP_CH_TX_POSTPONED_SF is already defined
#endif /* if (defined CANTP_CH_TX_POSTPONED_SF) */
/** \brief tx channel - set postpone flag for SF */
#define CANTP_CH_TX_POSTPONED_SF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_FRAME_SF)

#if (defined CANTP_CH_TX_POSTPONED_FF) /* to prevent double declaration */
#error CANTP_CH_TX_POSTPONED_FF is already defined
#endif /* if (defined CANTP_CH_TX_POSTPONED_FF) */
/** \brief tx channel - set postpone flag for FF */
#define CANTP_CH_TX_POSTPONED_FF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_FRAME_FF)

#if (defined CANTP_CH_TX_POSTPONED_CF) /* to prevent double declaration */
#error CANTP_CH_TX_POSTPONED_CF is already defined
#endif /* if (defined CANTP_CH_TX_POSTPONED_CF) */
/** \brief tx channel - set postpone flag for CF */
#define CANTP_CH_TX_POSTPONED_CF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_POSTPONE_FRAME | CANTP_CH_FRAME_CF)

#if (defined CANTP_CH_TX_BUFREQ_SF) /* to prevent double declaration */
#error CANTP_CH_TX_BUFREQ_SF is already defined
#endif /* if (defined CANTP_CH_TX_BUFREQ_SF) */
/** \brief tx channel - request tx buffer for SF */
#define CANTP_CH_TX_BUFREQ_SF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_BUFREQ   | CANTP_CH_FRAME_SF)

#if (defined CANTP_CH_TX_BUFREQ_FF) /* to prevent double declaration */
#error CANTP_CH_TX_BUFREQ_FF is already defined
#endif /* if (defined CANTP_CH_TX_BUFREQ_FF) */
/** \brief tx channel - request tx buffer for FF */
#define CANTP_CH_TX_BUFREQ_FF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_BUFREQ   | CANTP_CH_FRAME_FF)

#if (defined CANTP_CH_TX_BUFREQ_CF) /* to prevent double declaration */
#error CANTP_CH_TX_BUFREQ_CF is already defined
#endif /* if (defined CANTP_CH_TX_BUFREQ_CF) */
/** \brief tx channel - request tx buffer for CF */
#define CANTP_CH_TX_BUFREQ_CF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_BUFREQ   | CANTP_CH_FRAME_CF)

#if (defined CANTP_CH_TX_CONFIRM_SF) /* to prevent double declaration */
#error CANTP_CH_TX_CONFIRM_SF is already defined
#endif /* if (defined CANTP_CH_TX_CONFIRM_SF) */
/** \brief tx channel - wait for tx SF confirmation */
#define CANTP_CH_TX_CONFIRM_SF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_CONFIRM  | CANTP_CH_FRAME_SF)

#if (defined CANTP_CH_TX_CONFIRM_FF) /* to prevent double declaration */
#error CANTP_CH_TX_CONFIRM_FF is already defined
#endif /* if (defined CANTP_CH_TX_CONFIRM_FF) */
/** \brief tx channel - wait for tx FF confirmation */
#define CANTP_CH_TX_CONFIRM_FF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_CONFIRM  | CANTP_CH_FRAME_FF)

#if (defined CANTP_CH_TX_CONFIRM_CF) /* to prevent double declaration */
#error CANTP_CH_TX_CONFIRM_CF is already defined
#endif /* if (defined CANTP_CH_TX_CONFIRM_CF) */
/** \brief tx channel - wait for tx CF confirmation */
#define CANTP_CH_TX_CONFIRM_CF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_CONFIRM  | CANTP_CH_FRAME_CF)

#if (defined CANTP_CH_TX_RECEIVE_FC) /* to prevent double declaration */
#error CANTP_CH_TX_RECEIVE_FC is already defined
#endif /* if (defined CANTP_CH_TX_RECEIVE_FC) */
/** \brief tx channel - receive FC next */
#define CANTP_CH_TX_RECEIVE_FC \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_RECEIVE  | CANTP_CH_FRAME_FC_CTS)

#if (defined CANTP_CH_TX_WAITTMR_CF) /* to prevent double declaration */
#error CANTP_CH_TX_WAITTMR_CF is already defined
#endif /* if (defined CANTP_CH_TX_WAITTMR_CF) */
/** \brief tx channel - wait StMin before transmit CF */
#define CANTP_CH_TX_WAITTMR_CF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_WAITTMR  | CANTP_CH_FRAME_CF)

#if (defined CANTP_CH_TX_REQUESTEDBUF_CF) /* to prevent double declaration */
#error CANTP_CH_TX_REQUESTEDBUF_CF is already defined
#endif /* if (defined CANTP_CH_TX_REQUESTEDBUF_CF) */
/** \brief tx channel - wait StMin before transmit CF */
#define CANTP_CH_TX_REQUESTEDBUF_CF \
    (uint8)(CANTP_CH_MODE_TX | CANTP_CH_ACTION_REQUESTED_FCBUFFER  | CANTP_CH_FRAME_CF)

/* access macros to retrieve substate information */

#if (defined CANTP_CH_GET_MODE) /* to prevent double declaration */
#error CANTP_CH_GET_MODE is already defined
#endif /* if (defined CANTP_CH_GET_MODE) */
/** \brief get mode substate for ChannelPtr */
#define CANTP_CH_GET_MODE(ChannelState) \
    ((uint8) ((ChannelState) & CANTP_CH_MODE_MASK))

#if (defined CANTP_CH_GET_ACTION) /* to prevent double declaration */
#error CANTP_CH_GET_ACTION is already defined
#endif /* if (defined CANTP_CH_GET_ACTION) */
/** \brief get action substate for ChannelPtr */
#define CANTP_CH_GET_ACTION(ChannelState) \
    ((uint8) ((ChannelState) & CANTP_CH_ACTION_MASK))

#if (defined CANTP_CALCULATE_CANID_METADATA) /* to prevent double declaration */
#error CANTP_CALCULATE_CANID_METADATA is already defined
#endif /* if (defined CANTP_CALCULATE_CANID_METADATA) */
/** \brief get the CanID based on the N_TA and N_SA */
#define CANTP_CALCULATE_CANID_METADATA(tatype, nsa, nta) \
    ((tatype) ? ((uint32)((0x18DB0000U) | (uint32)((nta) << 8U) | (uint32)(nsa))) : \
                ((uint32)((0x18DA0000U) | (uint32)((nta) << 8U) | (uint32)(nsa))))

#if (defined CANTP_GET_RX_METADATA_N_SA) /* to prevent double declaration */
#error CANTP_GET_RX_METADATA_N_SA is already defined
#endif /* if (defined CANTP_GET_RX_METADATA_N_SA) */
/** \brief get the N_SA from the CanID (metadata) */
#define CANTP_GET_RX_METADATA_N_SA(frametype, metadata) \
    (((frametype) == CANTP_N_PCI_TYPE_FC) ? ((uint16)((uint16)((metadata) >> 8U) & 0xFFU)) : \
                                             ((uint16)((metadata) & 0xFFU)))

 #if (defined CANTP_GET_RX_METADATA_N_TA) /* to prevent double declaration */
#error CANTP_GET_RX_METADATA_N_TA is already defined
#endif /* if (defined CANTP_GET_RX_METADATA_N_TA) */
/** \brief get the N_TA from the CanID (metadata) */
#define CANTP_GET_RX_METADATA_N_TA(frametype, metadata) \
    (((frametype) == CANTP_N_PCI_TYPE_FC) ? ((uint16)((metadata) & 0xFFU)) : \
                                            ((uint16)((uint16)((metadata) >> 8U) & 0xFFU)))

#if (defined CANTP_CH_SET_STATEBITS) /* to prevent double declaration */
#error CANTP_CH_SET_STATEBITS is already defined
#endif /* if (defined CANTP_CH_SET_STATEBITS) */
/** \brief set state bits */
#define CANTP_CH_SET_STATEBITS(ChannelPtr, BitMask, BitsToSet)              \
  do                                                                        \
  {                                                                         \
    CanTp_ChStateType NextState;                                            \
    CanTp_ChStateType CurrentState;                                         \
    TS_AtomicAssign8(CurrentState, (CanTp_ChStateType) (ChannelPtr)->State);\
    NextState = CurrentState                                                \
             &(~(CanTp_ChStateType)(BitMask));                              \
    NextState = NextState | ((CanTp_ChStateType) (BitsToSet));              \
    DBG_CANTP_CHANNEL_STATE_GRP(                                            \
                                DBG_CANTP_GET_CHANNEL_INDEX((ChannelPtr)),  \
                                CurrentState,                               \
                                NextState                                   \
                               );                                           \
    TS_AtomicAssign8((ChannelPtr)->State, NextState);                       \
  } while(0U)

#if (defined CANTP_CH_SET_ACTION) /* to prevent double declaration */
#error CANTP_CH_SET_ACTION is already defined
#endif /* if (defined CANTP_CH_SET_ACTION) */


/** \brief set action substate of ChannelPtr to act */
#define CANTP_CH_SET_ACTION(ChannelPtr, act)                                  \
  do                                                                          \
  {                                                                           \
    CANTP_CH_SET_STATEBITS((ChannelPtr), CANTP_CH_ACTION_MASK, (act)); \
  } while(0U)

#if (defined CANTP_IS_TX_CH_CONFIRM_ACTION) /* to prevent double declaration */
#error CANTP_IS_TX_CH_CONFIRM_ACTION is already defined
#endif /* if (defined CANTP_IS_TX_CH_CONFIRM_ACTION) */
/* Check if a confirmation is awaited on the same Tx Pdu*/
#define CANTP_IS_TX_CH_CONFIRM_ACTION(chidx, isTxCh, retval)                               \
  do                                                                                       \
  {                                                                                        \
    CanTp_ChStateType ChanState;                                                           \
    P2VAR(CanTp_ChannelType, AUTOMATIC, CANTP_VAR) ChPtr;                                  \
    retval = FALSE;                                                                        \
    if(((isTxCh == TRUE) && (chidx != INVALID_TX_CHANNEL_ID)) ||                           \
       ((isTxCh == FALSE) && (chidx != INVALID_RX_CHANNEL_ID)))                            \
    {                                                                                      \
      ChPtr = &CanTp_Channel[chidx];                                                       \
      TS_AtomicAssign8(ChanState, ChPtr->State);                                           \
      if(CANTP_CH_GET_ACTION(ChanState) == CANTP_CH_ACTION_CONFIRM)                        \
      {                                                                                    \
        retval = TRUE;                                                                     \
      }                                                                                    \
    }                                                                                      \
  }while(0U)
/*------------------[frame types]--------------------------------------------*/

/** \brief defines for first N_PCI byte in TP frames */

#if (defined CANTP_N_PCI_TYPE_MASK) /* to prevent double declaration */
#error CANTP_N_PCI_TYPE_MASK is already defined
#endif /* if (defined CANTP_N_PCI_TYPE_MASK) */
#define CANTP_N_PCI_TYPE_MASK   0xF0U /* mask for N_PCItype */

#if (defined CANTP_N_PCI_TYPE_SF) /* to prevent double declaration */
#error CANTP_N_PCI_TYPE_SF is already defined
#endif /* if (defined CANTP_N_PCI_TYPE_SF) */
#define CANTP_N_PCI_TYPE_SF     0x00U /* N_PCItype = 0: SF */

#if (defined CANTP_N_PCI_TYPE_FF) /* to prevent double declaration */
#error CANTP_N_PCI_TYPE_FF is already defined
#endif /* if (defined CANTP_N_PCI_TYPE_FF) */
#define CANTP_N_PCI_TYPE_FF     0x10U /* N_PCItype = 0: FF */

#if (defined CANTP_N_PCI_TYPE_CF) /* to prevent double declaration */
#error CANTP_N_PCI_TYPE_CF is already defined
#endif /* if (defined CANTP_N_PCI_TYPE_CF) */
#define CANTP_N_PCI_TYPE_CF     0x20U /* N_PCItype = 0: CF */

#if (defined CANTP_N_PCI_TYPE_FC) /* to prevent double declaration */
#error CANTP_N_PCI_TYPE_FC is already defined
#endif /* if (defined CANTP_N_PCI_TYPE_FC) */
#define CANTP_N_PCI_TYPE_FC     0x30U /* N_PCItype = 0: FC */

#if (defined CANTP_N_PCI_FC_CTS) /* to prevent double declaration */
#error CANTP_N_PCI_FC_CTS is already defined
#endif /* if (defined CANTP_N_PCI_FC_CTS) */
#define CANTP_N_PCI_FC_CTS      0x30U /* N_PCItype FC and FlowStatus CTS */

#if (defined CANTP_N_PCI_FC_WT) /* to prevent double declaration */
#error CANTP_N_PCI_FC_WT is already defined
#endif /* if (defined CANTP_N_PCI_FC_WT) */
#define CANTP_N_PCI_FC_WT       0x31U /* N_PCItype FC and FlowStatus WT */

#if (defined CANTP_N_PCI_FC_OVFLW) /* to prevent double declaration */
#error CANTP_N_PCI_FC_OVFLW is already defined
#endif /* if (defined CANTP_N_PCI_FC_OVFLW) */
#define CANTP_N_PCI_FC_OVFLW    0x32U /* N_PCItype FC and FlowStatus OVFLW */

#if (defined CANTP_N_PCI_CF_SN_MASK) /* to prevent double declaration */
#error CANTP_N_PCI_CF_SN_MASK is already defined
#endif /* if (defined CANTP_N_PCI_CF_SN_MASK) */
#define CANTP_N_PCI_CF_SN_MASK  0x0FU /* N_PCItype CF: mask for SN */

#if (defined CANTP_N_PCI_SF_DL_MASK) /* to prevent double declaration */
#error CANTP_N_PCI_SF_DL_MASK is already defined
#endif /* if (defined CANTP_N_PCI_SF_DL_MASK) */
#define CANTP_N_PCI_SF_DL_MASK  0x0FU /* N_PCItype SF: mask for DL */

#if (defined CANTP_N_PCI_FF_DL_MASK) /* to prevent double declaration */
#error CANTP_N_PCI_FF_DL_MASK is already defined
#endif /* if (defined CANTP_N_PCI_FF_DL_MASK) */
#define CANTP_N_PCI_FF_DL_MASK  0x0FU /* N_PCItype FF: mask for DL (first 4 bits) */

/* macros to distinguish N_PCI byte */

#if (defined CANTP_IS_N_PCI_TYPE_FC) /* to prevent double declaration */
#error CANTP_IS_N_PCI_TYPE_FC is already defined
#endif /* if (defined CANTP_IS_N_PCI_TYPE_FC) */

/** \brief return true if npci has frametype FC */
#define CANTP_IS_N_PCI_TYPE_FC(npci) \
    (((npci) & CANTP_N_PCI_TYPE_MASK) == CANTP_N_PCI_TYPE_FC)

#if (defined CANTP_GET_N_PCI_TYPE) /* to prevent double declaration */
#error CANTP_GET_N_PCI_TYPE is already defined
#endif /* if (defined CANTP_GET_N_PCI_TYPE) */

/** \brief returns frame type */
#define CANTP_GET_N_PCI_TYPE(npci) \
    ((npci) & CANTP_N_PCI_TYPE_MASK)

/* macros to distinguish addressing formats */

#if (defined CANTP_IS_NORMAL_ADDRESSING) /* to prevent double declaration */
#error CANTP_IS_NORMAL_ADDRESSING is already defined
#endif /* if (defined CANTP_IS_NORMAL_ADDRESSING) */

#if(CANTP_DYNAMIC_ID_SUPPORT == STD_ON)
/** \brief return true if STANDARD or NORMALFIXED addressing format is used */
#define CANTP_IS_NORMAL_ADDRESSING(addformat) \
    (((addformat) == CANTP_STANDARD) || ((addformat) == CANTP_NORMALFIXED))
#else
/** \brief return true if STANDARD addressing format is used */
#define CANTP_IS_NORMAL_ADDRESSING(addformat) ((addformat) == CANTP_STANDARD)
#endif /* CANTP_DYNAMIC_ID_SUPPORT == STD_ON */

/*------------------[critical sections]--------------------------------------*/

#if (defined CANTP_LOCK_CHANNEL) /* to prevent double declaration */
#error CANTP_LOCK_CHANNEL is already defined
#endif /* if (defined CANTP_LOCK_CHANNEL) */

/** \brief macro CANTP_LOCK_CHANNEL
 **
 ** Locks a channel to ensure reentrancy protection for channel specific state
 ** machine. Concurrent events occuring while a channel is locked will be
 ** saved and executed when unlocking the channel.
 */
#define CANTP_LOCK_CHANNEL(chptr)  do  \
  {                                    \
    (chptr)->ChannelLockFlag = 1U;     \
  } while(0U)



#if (defined CANTP_IS_CHANNEL_LOCKED) /* to prevent double declaration */
#error CANTP_IS_CHANNEL_LOCKED is already defined
#endif /* if (defined CANTP_IS_CHANNEL_LOCKED) */


/** \brief macro CANTP_IS_CHANNEL_LOCKED
 **
 ** Returns true if the specified channel is locked, false otherwise.
 ** if the channel is locked, the caller has to save its event data in the
 ** channel structure.
 */
#define CANTP_IS_CHANNEL_LOCKED(chptr)  ((chptr)->ChannelLockFlag != 0U)


#if (defined CANTP_UNLOCK_CHANNEL) /* to prevent double declaration */
#error CANTP_UNLOCK_CHANNEL is already defined
#endif /* if (defined CANTP_UNLOCK_CHANNEL) */

/** \brief macro CANTP_UNLOCK_CHANNEL
 **
 ** Unlocks a previously locked channel and executes notification functions
 ** for channel specific events that occured in the meanwhile.
 **
 ** Note: implementation is done in the CanTp_UnlockChannel function
 */
#define CANTP_UNLOCK_CHANNEL(chptr)  do  \
  {                                            \
    (chptr)->ChannelLockFlag = 0U;      \
  } while(0U)

#if (defined CANTP_SET_FLAG) /* to prevent double declaration */
#error CANTP_SET_FLAG is already defined
#endif /* if (defined CANTP_SET_FLAG) */

/** \brief macro CANTP_SET_FLAG
 **
 ** Sets the specific flag for specific variable.
 **
 ** flag - specific flag
 ** mask - mask which have to be set
 */

#define CANTP_SET_FLAG(flag, mask)                                  \
  do                                                                          \
  {                                                                           \
    (flag) |= (mask); \
  } while(0U)

#if (defined CANTP_CLEAR_FLAG) /* to prevent double declaration */
#error CANTP_CLEAR_FLAG is already defined
#endif /* if (defined CANTP_CLEAR_FLAG) */

/** \brief macro CANTP_CLEAR_FLAG
 **
 ** Clear the specific flag for specific variable.
 **
 ** flag - specific flag
 ** mask - mask which have to be cleared
 */

#define CANTP_CLEAR_FLAG(flag, mask)                                  \
  do                                                                          \
  {                                                                           \
    (flag) &= ~(mask);                                          \
  } while(0U)

#if (defined CANTP_CHECK_FLAG) /* to prevent double declaration */
#error CANTP_CHECK_FLAG is already defined
#endif /* if (defined CANTP_CHECK_FLAG) */

/** \brief macro CANTP_CHECK_FLAG
 **
 ** Check if the specified flag is set.
 **
 ** flag - specific flag
 ** mask - mask which have to be checked
 ** retval - TRUE if the specified flag is set
 */

#define CANTP_CHECK_FLAG(flag, mask)       ((flag) & (mask))

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef CANTP_INTERNALLIB_H */
/*==================[end of file]============================================*/
