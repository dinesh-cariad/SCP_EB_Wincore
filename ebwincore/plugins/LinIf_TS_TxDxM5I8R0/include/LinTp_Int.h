/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef LINTP_INT_H
#define LINTP_INT_H

/*==================[inclusions]=============================================*/

#include <ComStack_Types.h>        /* AUTOSAR standard types */
#include <LinIf_Cfg.h>             /* Module static configuration */
#include <LinTp_Types.h>           /* LinTp basic types */

#if (LINIF_TP_SUPPORTED == STD_ON)

#if (defined TS_RELOCATABLE_CFG_ENABLE)
#error TS_RELOCATABLE_CFG_ENABLE already defined!
#endif /* #if (defined TS_RELOCATABLE_CFG_ENABLE)*/
#define TS_RELOCATABLE_CFG_ENABLE LINTP_RELOCATABLE_CFG_ENABLE

#include <TSPBConfig_Access.h>
#include <TSPBConfig_Signature.h>  /* post-build macros */

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

/*==================[macros]=================================================*/

/* EB specific LinTp module ID */
#if (defined LINTP_MODULE_ID)
#error LINTP_MODULE_ID already defined!
#endif /* #if (defined LINTP_MODULE_ID)*/
#define LINTP_MODULE_ID           0x8002U

/* API service IDs */
#if (defined LINTP_API_INIT)
#error LINTP_API_INIT already defined!
#endif /* #if (defined LINTP_API_INIT)*/
#define LINTP_API_INIT              0x40U

#if (defined LINTP_API_TRANSMIT)
#error LINTP_API_TRANSMIT already defined!
#endif /* #if (defined LINTP_API_TRANSMIT)*/
#define LINTP_API_TRANSMIT          0x41U

#if (defined LINTP_API_GETVERSIONINFO)
#error LINTP_API_GETVERSIONINFO already defined!
#endif /* #if (defined LINTP_API_GETVERSIONINFO)*/
#define LINTP_API_GETVERSIONINFO    0x42U

#if (defined LINTP_API_CHANGEPARAMREQEST)
#error LINTP_API_CHANGEPARAMREQEST already defined!
#endif /* #if (defined LINTP_API_CHANGEPARAMREQEST)*/
#define LINTP_API_CHANGEPARAMREQEST 0x44U

#if (defined LINTP_API_CANCELTXREQUEST)
#error LINTP_API_CANCELTXREQUEST already defined!
#endif /* #if (defined LINTP_API_CANCELTXREQUEST)*/
#define LINTP_API_CANCELTXREQUEST   0x46U

#if (defined LINTP_API_CANCELRXREQUEST)
#error LINTP_API_CANCELRXREQUEST already defined!
#endif /* #if (defined LINTP_API_CANCELRXREQUEST)*/
#define LINTP_API_CANCELRXREQUEST   0x47U

#if (defined LINTP_API_ISVALIDCONFIG)
#error LINTP_API_ISVALIDCONFIG already defined!
#endif /* #if (defined LINTP_API_ISVALIDCONFIG)*/
#define LINTP_API_ISVALIDCONFIG     0x48U

/* State macros for LinTp global FSM */
#if (defined LINTP_UNINIT)
#error LINTP_UNINIT already defined!
#endif /* #if (defined LINTP_UNINIT)*/
#define LINTP_UNINIT               0U

#if (defined LINTP_INIT)
#error LINTP_INIT already defined!
#endif /* #if (defined LINTP_INIT)*/
#define LINTP_INIT                 1U

/* Data length of Pdus */
#if (defined LINTP_PCI_DATA_LENGTH)
#error LINTP_PCI_DATA_LENGTH already defined!
#endif /* #if (defined LINTP_PCI_DATA_LENGTH)*/
#define LINTP_PCI_DATA_LENGTH             6U

#if (defined LINTP_PCI_FF_DATA_LENGTH)
#error LINTP_PCI_FF_DATA_LENGTH already defined!
#endif /* #if (defined LINTP_PCI_FF_DATA_LENGTH)*/
#define LINTP_PCI_FF_DATA_LENGTH          5U

/* Minimal data length of a split message */
#if (defined LINTP_FF_DATA_MIN_LENGTH)
#error LINTP_FF_DATA_MIN_LENGTH already defined!
#endif /* #if (defined LINTP_FF_DATA_MIN_LENGTH)*/
#define LINTP_FF_DATA_MIN_LENGTH          7U

/* Data length of a Pdu */
#if (defined LINTP_PDU_LENGTH)
#error LINTP_PDU_LENGTH already defined!
#endif /* #if (defined LINTP_PDU_LENGTH)*/
#define LINTP_PDU_LENGTH                  8U

/* Send data with payload of 0 bytes (get available data length) */
#if (defined LINTP_EMPTY_SDU_LENGTH)
#error LINTP_EMPTY_SDU_LENGTH already defined!
#endif /* #if (defined LINTP_EMPTY_SDU_LENGTH)*/
#define LINTP_EMPTY_SDU_LENGTH            0U

/* Max sequence number of CF */
#if (defined LINTP_CF_MAX_SEQUENCE_NUMBER)
#error LINTP_CF_MAX_SEQUENCE_NUMBER already defined!
#endif /* #if (defined LINTP_CF_MAX_SEQUENCE_NUMBER)*/
#define LINTP_CF_MAX_SEQUENCE_NUMBER     15U

/* Upper range of valid SID */
#if (defined LINTP_STANDARD_SID_UPPER_LIMIT)
#error LINTP_STANDARD_SID_UPPER_LIMIT already defined!
#endif /* #if (defined LINTP_STANDARD_SID_UPPER_LIMIT)*/
#define LINTP_STANDARD_SID_UPPER_LIMIT        0x7EU

/* Invalid SID*/
#if (defined LINTP_INVALID_SID)
#error LINTP_INVALID_SID already defined!
#endif /* #if (defined LINTP_INVALID_SID)*/
#define LINTP_INVALID_SID                0xFFU

/* Invalid NAD */
#if (defined LINTP_INVALID_NAD)
#error LINTP_INVALID_NAD already defined!
#endif /* #if (defined LINTP_INVALID_NAD)*/
#define LINTP_INVALID_NAD                0x00U

/* Invalid SDU ID */
#if (defined LINTP_INVALID_SDU_ID)
#error LINTP_INVALID_SDU_ID already defined!
#endif /* #if (defined LINTP_INVALID_SDU_ID)*/
#define LINTP_INVALID_SDU_ID             0xFFFFU

/* Bit masks of PCI */
#if (defined LINTP_PCI_SF_FRAME)
#error LINTP_PCI_SF_FRAME already defined!
#endif /* #if (defined LINTP_PCI_SF_FRAME)*/
#define LINTP_PCI_SF_FRAME                0x00U

#if (defined LINTP_PCI_FF_FRAME)
#error LINTP_PCI_FF_FRAME already defined!
#endif /* #if (defined LINTP_PCI_FF_FRAME)*/
#define LINTP_PCI_FF_FRAME                0x10U

#if (defined LINTP_PCI_CF_FRAME)
#error LINTP_PCI_CF_FRAME already defined!
#endif /* #if (defined LINTP_PCI_CF_FRAME)*/
#define LINTP_PCI_CF_FRAME                0x20U

/* max message length */
#if (defined LINTP_MAX_MSG_LENGTH)
#error LINTP_MAX_MSG_LENGTH already defined!
#endif /* #if (defined LINTP_MAX_MSG_LENGTH)*/
#define LINTP_MAX_MSG_LENGTH                     4095U

/* Flag which indicated that no request is pending */
#if (defined LINTP_FLAGS_CLEARED)
#error LINTP_FLAGS_CLEARED already defined!
#endif /* #if (defined LINTP_FLAGS_CLEARED)*/
#define LINTP_FLAGS_CLEARED                      0x00U

/* Mask to clear BswM schedule change flags except for the P2 timeout one */
#if (defined LINTP_BSWM_SCHEDULE_SWITCH_NOT_FROM_P2_FLAGS_MASK)
#error LINTP_BSWM_SCHEDULE_SWITCH_NOT_FROM_P2_FLAGS_MASK already defined!
#endif /* #if (defined LINTP_BSWM_SCHEDULE_SWITCH_NOT_FROM_P2_FLAGS_MASK)*/
#define LINTP_BSWM_SCHEDULE_SWITCH_NOT_FROM_P2_FLAGS_MASK    0x70U

/* Mask to clear BswM schedule change flags */
#if (defined LINTP_BSWM_SCHEDULE_SWITCH_FALGS_MASK)
#error LINTP_BSWM_SCHEDULE_SWITCH_FALGS_MASK already defined!
#endif /* #if (defined LINTP_BSWM_SCHEDULE_SWITCH_FALGS_MASK)*/
#define LINTP_BSWM_SCHEDULE_SWITCH_FALGS_MASK                0xF0U

/* Flag which indicates that a physical transmission was requested */
#if (defined LINTP_FLAG_PHYS_TRANSMIT_REQUESTED)
#error LINTP_FLAG_PHYS_TRANSMIT_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_PHYS_TRANSMIT_REQUESTED)*/
#define LINTP_FLAG_PHYS_TRANSMIT_REQUESTED       0x01U

/* Flag which indicates that current Rx transmission should be aborted */
#if (defined LINTP_FLAG_CANCEL_RECEIVE_REQUESTED)
#error LINTP_FLAG_CANCEL_RECEIVE_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_CANCEL_RECEIVE_REQUESTED)*/
#define LINTP_FLAG_CANCEL_RECEIVE_REQUESTED      0x02U

/* Flag which indicates that an ongoing reception can be aborted */
#if (defined LINTP_FLAG_CANCEL_RECEIVE_ALLOWED)
#error LINTP_FLAG_CANCEL_RECEIVE_ALLOWED already defined!
#endif /* #if (defined LINTP_FLAG_CANCEL_RECEIVE_ALLOWED)*/
#define LINTP_FLAG_CANCEL_RECEIVE_ALLOWED        0x04U

/* Flag which indicates that a functional transmission was requested */
#if (defined LINTP_FLAG_FUNC_TRANSMIT_REQUESTED)
#error LINTP_FLAG_FUNC_TRANSMIT_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_FUNC_TRANSMIT_REQUESTED)*/
#define LINTP_FLAG_FUNC_TRANSMIT_REQUESTED       0x08U
/* Flag which indicates that a BswM schedule change from a functional transmission was requested */
#if (defined LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED)
#error LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED)*/
#define LINTP_FLAG_FUNC_BSWM_SCHEDULE_CHANGE_REQUESTED       0x10U

/* Flag which indicates that a BswM schedule change from a physical transmission was requested */
#if (defined LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED)
#error LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED)*/
#define LINTP_FLAG_PHYS_BSWM_SCHEDULE_CHANGE_REQUESTED       0x20U

/* Flag which indicates that a BswM schedule change from a physical reception was requested */
#if (defined LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED)
#error LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED)*/
#define LINTP_FLAG_PHYS_RX_BSWM_SCHEDULE_CHANGE_REQUESTED    0x40U

/* Flag which indicates P2 timout happend while transitioning to response schedule table */
#if (defined LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED)
#error LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED already defined!
#endif /* #if (defined LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED)*/
#define LINTP_FLAG_P2_BSWM_SCHEDULE_CHANGE_REQUESTED         0x80U

#if (defined LINTP_UNINIT_ACCESS)
#error LINTP_UNINIT_ACCESS already defined!
#endif /* #if (defined LINTP_UNINIT_ACCESS)*/
#define LINTP_UNINIT_ACCESS \
((LinTp_InitState != LINTP_INIT) || (LinTp_ConfigPtr == NULL_PTR))

#if (LINIF_TP_SUPPORTED == STD_ON)

#if (defined LINTP_STATUS_ERR)
#error LINTP_STATUS_ERR already defined!
#endif /* #if (defined LINTP_STATUS_ERR)*/
#define LINTP_STATUS_ERR          &LinTp_Status_Err

#if (defined LINTP_STATUS_ERR_NO_DET)
#error LINTP_STATUS_ERR_NO_DET already defined!
#endif /* #if (defined LINTP_STATUS_ERR_NO_DET)*/
#define LINTP_STATUS_ERR_NO_DET   &LinTp_Err_Tx

#if (defined LINTP_TX_CONFIRMATION)
#error LINTP_TX_CONFIRMATION already defined!
#endif /* #if (defined LINTP_TX_CONFIRMATION)*/
#define LINTP_TX_CONFIRMATION     &LinTp_TxConfirmation

#if (defined LINTP_RX_INDICATION)
#error LINTP_RX_INDICATION already defined!
#endif /* #if (defined LINTP_RX_INDICATION)*/
#define LINTP_RX_INDICATION       &LinTp_RxIndication

#else /* (LINIF_TP_SUPPORTED == STD_ON) */

#if (defined LINTP_STATUS_MRF_BUSY_ERR)
#error LINTP_STATUS_MRF_BUSY_ERR already defined!
#endif /* #if (defined LINTP_STATUS_MRF_BUSY_ERR)*/
#define LINTP_STATUS_MRF_BUSY_ERR NULL_PTR

#if (defined LINTP_STATUS_ERR)
#error LINTP_STATUS_ERR already defined!
#endif /* #if (defined LINTP_STATUS_ERR)*/
#define LINTP_STATUS_ERR          NULL_PTR

#if (defined LINTP_STATUS_ERR_NO_DET)
#error LINTP_STATUS_ERR_NO_DET already defined!
#endif /* #if (defined LINTP_STATUS_ERR_NO_DET)*/
#define LINTP_STATUS_ERR_NO_DET    NULL_PTR

#if (defined LINTP_TX_CONFIRMATION)
#error LINTP_TX_CONFIRMATION already defined!
#endif /* #if (defined LINTP_TX_CONFIRMATION)*/
#define LINTP_TX_CONFIRMATION     NULL_PTR

#if (defined LINTP_RX_INDICATION)
#error LINTP_RX_INDICATION already defined!
#endif /* #if (defined LINTP_RX_INDICATION)*/
#define LINTP_RX_INDICATION       NULL_PTR

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

/*---------------[Relocatable post build configuration macros]----------------*/

#if (LINIF_TP_SUPPORTED == STD_ON)

/** \brief Get a const reference to a post-build RAM element of the LinTp
 **
 ** This macro retrieves a const pointer to a post-build RAM element of the LinTp
 */
#if (defined LINTP_PBCFG_ACCESS)
#error LINTP_PBCFG_ACCESS already defined!
#endif /* #if (defined LINTP_PBCFG_ACCESS)*/
#define LINTP_PBCFG_ACCESS(type, element)\
    TS_UNCHECKEDGETCFG(LinTp_ConfigPtr, type, LINTP, LinTp_ConfigPtr->element)

/** \brief Macro to get TxNSduRef
 **
 ** This macro retrieves TxNSduRef to the post-build RAM element
 */
#if (defined LINTP_CFG_GET_TXNSDU)
#error LINTP_CFG_GET_TXNSDU already defined!
#endif /* #if (defined LINTP_CFG_GET_TXNSDU)*/
#define LINTP_CFG_GET_TXNSDU \
   (&LINTP_PBCFG_ACCESS(LinTpTxNSduType, TxNSduRef)[0])

/** \brief Macro to get RxNSduRef
 **
 ** This macro retrieves RxNSduRef to the post-build RAM element
 */
#if (defined LINTP_CFG_GET_RXNSDU)
#error LINTP_CFG_GET_RXNSDU already defined!
#endif /* #if (defined LINTP_CFG_GET_RXNSDU)*/
#define LINTP_CFG_GET_RXNSDU \
   (&LINTP_PBCFG_ACCESS(LinTpRxNSduType, RxNSduRef)[0])

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

/** \brief Current PCI states
 **/
#if (defined LINTP_PCI_FF)
#error LINTP_PCI_FF already defined!
#endif /* #if (defined LINTP_PCI_FF)*/
#define LINTP_PCI_FF 0U               /**< First Frame */

#if (defined LINTP_PCI_SF)
#error LINTP_PCI_SF already defined!
#endif /* #if (defined LINTP_PCI_SF)*/
#define LINTP_PCI_SF 1U               /**< Single Frame */

#if (defined LINTP_PCI_CF)
#error LINTP_PCI_CF already defined!
#endif /* #if (defined LINTP_PCI_CF)*/
#define LINTP_PCI_CF 2U               /**< Consecutive Frame */

#if (defined LINTP_PCI_INVALID)
#error LINTP_PCI_INVALID already defined!
#endif /* #if (defined LINTP_PCI_INVALID)*/
#define LINTP_PCI_INVALID 3U          /**< PCI Invalid */


/** \brief Current LinTp states
 */
#if (defined LINTP_PHYS_STATE_IDLE)
#error LINTP_PHYS_STATE_IDLE already defined!
#endif /* #if (defined LINTP_PHYS_STATE_IDLE)*/
#define LINTP_PHYS_STATE_IDLE 0U      /**< currently no ongoing transmission, or reception of SF, FF*/

#if (defined LINTP_PHYS_STATE_WAITTX)
#error LINTP_PHYS_STATE_WAITTX already defined!
#endif /* #if (defined LINTP_PHYS_STATE_WAITTX)*/
#define LINTP_PHYS_STATE_WAITTX 1U    /**< sending of SF, FF, CF */

#if (defined LINTP_PHYS_STATE_TXPENDING)
#error LINTP_PHYS_STATE_TXPENDING already defined!
#endif /* #if (defined LINTP_PHYS_STATE_TXPENDING)*/
#define LINTP_PHYS_STATE_TXPENDING 2U /**< waiting for response of TX transmission */

#if (defined LINTP_PHYS_STATE_WAITRX)
#error LINTP_PHYS_STATE_WAITRX already defined!
#endif /* #if (defined LINTP_PHYS_STATE_WAITRX)*/
#define LINTP_PHYS_STATE_WAITRX 3U    /**< reception of CF */


/** \brief Current LinTp transmission states
 */

#if (defined LINTP_FUNC_STATE_IDLE)
#error LINTP_FUNC_STATE_IDLE already defined!
#endif /* #if (defined LINTP_FUNC_STATE_IDLE)*/
#define LINTP_FUNC_STATE_IDLE 0U      /**< no ongoing functional transmission */

#if (defined LINTP_FUNC_STATE_WAITTX)
#error LINTP_FUNC_STATE_WAITTX already defined!
#endif /* #if (defined LINTP_FUNC_STATE_WAITTX)*/
#define LINTP_FUNC_STATE_WAITTX 1U    /**< functional transmission */

#if (defined LINTP_FUNC_STATE_TXPENDING)
#error LINTP_FUNC_STATE_TXPENDING already defined!
#endif /* #if (defined LINTP_FUNC_STATE_TXPENDING)*/
#define LINTP_FUNC_STATE_TXPENDING 2U /**< functional transmission confirmation */


/** \brief Invalid PduR Result
 */
#if (defined LINTP_INVALID_PDUR_RESULT)
#error LINTP_INVALID_PDUR_RESULT already defined!
#endif /* #if (defined LINTP_INVALID_PDUR_RESULT)*/
#define LINTP_INVALID_PDUR_RESULT      0xFFU

 /** \brief No PduR Result
 */
#if (defined LINTP_NO_PDUR_RESULT)
#error LINTP_NO_PDUR_RESULT already defined!
#endif /* #if (defined LINTP_NO_PDUR_RESULT)*/
#define LINTP_NO_PDUR_RESULT           0xFEU

/** \brief Response of Pdu states
 */
#if (defined LINTP_NORMAL_RESPONSE)
#error LINTP_NORMAL_RESPONSE already defined!
#endif /* #if (defined LINTP_NORMAL_RESPONSE)*/
#define LINTP_NORMAL_RESPONSE 0U  /**< received valid response on a standard diagnostc service or
                                       received response on a user defined service */

#if (defined LINTP_PENDING_RESPONSE)
#error LINTP_PENDING_RESPONSE already defined!
#endif /* #if (defined LINTP_PENDING_RESPONSE)*/
#define LINTP_PENDING_RESPONSE 1U /**< received RSID is negative response + response pending */

/** \brief States of received CF
 */
#if (defined LINTP_OK)
#error LINTP_OK already defined!
#endif /* #if (defined LINTP_OK)*/
#define LINTP_OK 0U             /**< received CF is correct */

#if (defined LINTP_E_NOT_OK)
#error LINTP_E_NOT_OK already defined!
#endif /* #if (defined LINTP_E_NOT_OK)*/
#define LINTP_E_NOT_OK 1U       /**< unexpected Pdu */

#if (defined LINTP_E_SN)
#error LINTP_E_SN already defined!
#endif /* #if (defined LINTP_E_SN)*/
#define LINTP_E_SN 2U           /**< sequence number is wrong */

#if (defined LINTP_E_PDUR)
#error LINTP_E_PDUR already defined!
#endif /* #if (defined LINTP_E_PDUR)*/
#define LINTP_E_PDUR 3U         /**< error occurred during PduR call */

#if (defined LINTP_NEW_RECEPTION)
#error LINTP_NEW_RECEPTION already defined!
#endif /* #if (defined LINTP_NEW_RECEPTION)*/
#define LINTP_NEW_RECEPTION 4U  /**< SF or FF was received instead of CF,
                                     initiate new reception*/
/*==================[type definitions]=======================================*/

/** \brief Current PCI type
 *  Indicates the PCI type in the current transmission
 */
typedef uint8 LinTp_PCIType;

/** \brief Current LinTp state type
 *  Indicates the current state of the LinTp
 */
typedef uint8 LinTp_PhysStates_Type;

/** \brief Current LinTp transmission type
 *  Indicates if a functional request transmission is ongoing
 */
typedef uint8 LinTp_FuncStates_Type;

/** \brief Response type of Pdu
 * Indicates if received RSID is negative or positive response
 */
typedef uint8 LinTp_Response_Type;

/** \brief State of received CF type
 * Indicates if CF was received successfully
 */
typedef uint8 LinTp_FrameReturnType;

/** \brief Information about physical tx and rx transmissions
 **
 ** This structure gathers all information for one channel required during
 ** physical transmission of Pdus
 */
typedef struct
{
    PduLengthType PduRBufferAvailable;    /**< available data of PduR buffer */
    /* !LINKSTO LinIf.ASR40.LINIF315,1 */
    uint16 MsgLengthTotal;                /**< Total message length of current physical transmission */
    uint16 MsgLengthRemain;               /**< remaining message length to send/receive */
    uint16 PendingMsgLength;              /**< Saves total message length of a pending physical transmission */
    uint16 NumberOfRespPendingFrames;     /**< current counter state of respond pending frames */
    uint16 Timer;                         /**< current timer counter state of physical transmission */
    uint16 P2Timer;                       /**< current P2 timer counter state of physical transmission */
    PduIdType SduId;                      /**< LinTpSduId of current physical transmission */
    PduIdType PendingSduId;               /**< Saves LinTpSduId of a pending physical transmission request */
    LinTp_PhysStates_Type State;          /**< current physical transmission state of the LinTp */
    uint8 PduRResult;                     /**< PduR result for end of transmission */
    uint8 FrameCounter;                   /**< current frame number for CF */
    uint8 NAD;                            /**< NAD of current physical transmission */
    uint8 SID;                            /**< SID of current physical transmission */

} LinTp_PhysChannelInfoType;

/** \brief Information about functional transmission
 **
 ** This structure gathers all information for one channel required during
 ** a functional transmission
 */
typedef struct
{
    /* !LINKSTO LinIf.ASR40.LINIF315,1 */
    uint16 Timer;                         /**< current timer counter state of functional transmission */
    PduIdType SduId;                      /**< LinTpSduId of current functional transmission */
    LinTp_FuncStates_Type State;          /**< current functional transmission state of LinTp */
    uint8 MsgLengthTotal;                 /**< Total message length of current functional transmission */
    uint8 PduRResult;                     /**< PduR result for the end of transmission */
} LinTp_FuncChannelInfoType;

/** \brief Information about ongoing transmission
 **
 ** This structure gathers all information for one channel required during
 ** a onging transmission
 */
typedef struct
{
    LinTp_PhysChannelInfoType PhysChannelInfo;     /**< Provides information of an ongoing physical transmission */
#if (LINIF_MASTER_SUPPORTED == STD_ON)
    LinTp_FuncChannelInfoType FuncChannelInfo;     /**< Provides information of an ongoing functional transmission */
#endif
    uint8 ChannelFlags;                            /**< Bit0: 1 - Physical transmission requested
                                                          0 - There is no pending physical transmission request
                                                    Bit1: 1 - Cancel reception requested
                                                          0 - There is no pending cancel reception request
                                                    Bit2: 1 - Cancel reception accepted
                                                          0 - Cancel reception not accepted
                                                    Bit3: 1 - Functional transmission requested
                                                          0 - There is no pending functional request
                                                    Bit4: 1 - BswM Schedule change requested from end of functional transmission
                                                          0 - There is no pending BswM schedule change request from end of functional transmission
                                                    Bit5: 1 - BswM Schedule change requested from end of physical transmission
                                                          0 - There is no pending BswM schedule change request from end of physical transmission
                                                    Bit6: 1 - BswM Schedule change requested from end of physical receive
                                                          0 - There is no pending BswM schedule change request from end of physical receive
                                                    Bit7: 1 - P2 Timeout expired during schedule switch, BswM Schedule change needs to be requested after
                                                          0 - P2 Timeout not expired during schedule switch, BswM Schedule change does not need to be requested */

} LinTp_ChannelInfoType;


/*==================[external variable declarations]=========================*/

#if (LINIF_TP_SUPPORTED == STD_ON)

#define LINIF_START_SEC_VAR_INIT_8
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/** \brief Global state of the LIN Transport Layer */
extern VAR(uint8, LINIF_VAR) LinTp_InitState;

#define LINIF_STOP_SEC_VAR_INIT_8
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>


#define LINIF_START_SEC_VAR_CLEARED_UNSPECIFIED
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/** \brief Runtime information for TP channels */
extern VAR(LinTp_ChannelInfoType, LINIF_VAR) LinTp_ChannelInfo[LINIF_MAX_CHANNELS];

/** \brief This pointer holds the root of the configuration for the TP. */
extern P2CONST(LinTp_ConfigType, LINIF_VAR, LINIF_VAR) LinTp_ConfigPtr;

#define LINIF_STOP_SEC_VAR_CLEARED_UNSPECIFIED
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

/*==================[external function declarations]=========================*/

#if (LINIF_TP_SUPPORTED == STD_ON)

#define LINIF_START_SEC_CODE
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

/** \brief If MRF is the next frame which should be sent over the bus
 *         this function is called to get data from the PduR
 *
 *  \param currentChannel Channel which is processed
 *  \param pPduInfo       Data which is copied from the PduR
 *
 *  \return Result of operation
 *  \retval E_OK      Data was copied successfully
 *  \retval E_NOT_OK  Error occurred during copying
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_TriggerTransmit
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief Check that an active BswM schedule change request
 *         has been finalized
 *
 *  \param currentChannel Channel which is processed
 */
extern FUNC(void, LINIF_CODE) LinTp_BswMScheduleChangePerformed
(
    uint8 currentChannel
);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief If SRF is the next frame which should be sent over the bus
 *         this function checks if PduR is ready to receive data
 *
 *  \param currentChannel Channel which is processed
 *
 *  \return Result of operation
 *  \retval E_OK      PduR is ready to receive data
 *  \retval E_NOT_OK  Not enough space available
 */
extern FUNC(Std_ReturnType, LINIF_CODE) LinTp_TriggerReceive
(
    uint8 currentChannel
);


/** \brief This function is called after the status check of the bus.
 *         It finishes or triggers the next frame of a TX transmission
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        data which was received from the bus
 *
 */
extern FUNC(void, LINIF_CODE) LinTp_TxConfirmation
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);


/** \brief This function is called after a response is received over
 *         the bus, if the received data is valid it is copied to the PduR
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        data which was received from the bus
 *
 */
extern FUNC(void, LINIF_CODE) LinTp_RxIndication
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#if (LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief The TxRxMainFunction introduce physical and
 *         functional Tx transmissions and cancel ongoing receptions
 *
 *  \param currentChannel Channel which is processed
 */
extern FUNC(void, LINIF_CODE) LinTp_TxRxMainFunction
(
    uint8 currentChannel
);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

/** \brief The TimerMainFunction handles timeouts
 *
 *  \param currentChannel Channel which is processed
 */
extern FUNC(void, LINIF_CODE) LinTp_TimerMainFunction
(
    uint8 currentChannel
);


/** \brief This function aborts a Tx transmission if
 *         Lin_GetStatus returns LIN_TX_HEADER_ERROR or LIN_TX_ERROR
 *         or LIN_TX_BIT_ERROR
 *
 *         Calls Det_ReportRuntimeError
 *
 *  \param currentChannel  Channel which is processed
 *  \param pPduInfo        data which was received from the bus
 */
extern FUNC(void, LINIF_CODE) LinTp_Status_Err
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

/** \brief This function aborts a Tx transmission if
 *         Lin_GetStatus returns LIN_TX_HEADER_ERROR or LIN_TX_ERROR
 *         or LIN_TX_BIT_ERROR
 *
 *  \param currentChannel  Channel which is processed
 */
extern FUNC(void, LINIF_CODE) LinTp_Err_Tx
(
    uint8 currentChannel,
    P2VAR(PduInfoType,AUTOMATIC,LINIF_APPL_DATA) pPduInfo
);

#if (LINIF_SLAVE_SUPPORTED == STD_ON)
/** \brief This function aborts a Rx reception if
 *         LinIf_LinErrorIndication is called with LIN_ERR_RESP_STOPBIT or LIN_ERR_RESP_CHKSUM
 *         or LIN_ERR_INC_RESP or LIN_ERR_NO_RESP
 *
 *  \param currentChannel  Channel which is processed
 */
extern FUNC(void, LINIF_CODE) LinTp_Err_Rx
(
    uint8 currentChannel
);
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

/** \brief This function cancel an ongoing reception if
 *         LinTp_CanelReceive was called
 *
 *  \param currentChannel Channel where reception is aborted
 */
extern FUNC(void, LINIF_CODE) LinTp_CancelReceiveHandling
(
    uint8 currentChannel
);

#if(LINIF_MASTER_SUPPORTED == STD_ON)
/** \brief This function handles functional transmission requests
 *
 *  \param currentChannel Channel where functional transmission was requested
 */
extern FUNC(void, LINIF_CODE) LinTp_FuncTransmitHandling
(
    uint8 currentChannel
);

/** \brief This function handles physical transmission requests
 *
 *  \param currentChannel Channel where physical transmission was requested
 */
extern FUNC(void, LINIF_CODE) LinTp_PhysTransmitHandling
(
    uint8 currentChannel
);
#endif /* LINIF_MASTER_SUPPORTED == STD_ON */

#if(LINIF_SLAVE_SUPPORTED == STD_ON)
extern FUNC(void, LINIF_CODE) LinTp_SlavePhysTransmitHandling
(
    uint8 currentChannel
);

extern FUNC(uint8, LINIF_CODE) LinTp_GetConfNad
(
    uint8 currentChannel
);
#endif /* LINIF_SLAVE_SUPPORTED == STD_ON */

#define LINIF_STOP_SEC_CODE
/* !LINKSTO LinIf.ASR40.LINIF590,1 */
#include <LinIf_MemMap.h>

#undef TS_RELOCATABLE_CFG_ENABLE

#endif /* (LINIF_TP_SUPPORTED == STD_ON) */

#endif /* ifndef LINTP_INT_H */
/*==================[end of file]============================================*/
