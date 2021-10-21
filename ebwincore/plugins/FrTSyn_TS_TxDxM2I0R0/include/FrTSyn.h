/**
 * \file
 *
 * \brief AUTOSAR FrTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTSyn.
 *
 * \version 2.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRTSYN_H
#define FRTSYN_H

/*==================[inclusions]============================================*/
#include <TSAutosar.h> /* Specific standard types */
#include <FrTSyn_Cfg.h> /* Generated configuration */

/*==================[macros]================================================*/

/** \brief Module instance ID
 *
 * Defines the instance number of this module. Since multiple instances are not supported this ID
 * is always zero.
 **/
#if (defined FRTSYN_INSTANCE_ID)
#error FRTSYN_INSTANCE_ID already defined!
#endif /* #if (defined FRTSYN_INSTANCE_ID)*/
#define FRTSYN_INSTANCE_ID 0U

/** \brief Defines API id of function FrTSyn_Init(). */
#if (defined FRTSYN_SID_INIT)
#error FRTSYN_SID_INIT already defined!
#endif /* #if (defined FRTSYN_SID_INIT)*/
#define FRTSYN_SID_INIT 0x01U

/** \brief Defines API id of function FrTSyn_GetVersionInfo(). */
#if (defined FRTSYN_SID_GETVERSIONINFO)
#error FRTSYN_SID_GETVERSIONINFO already defined!
#endif /* #if (defined FRTSYN_SID_GETVERSIONINFO)*/
#define FRTSYN_SID_GETVERSIONINFO 0x02U

/** \brief Defines API id of function FrTSyn_SetTransmissionMode(). */
#if (defined FRTSYN_SID_SETTRANSMISSIONMODE)
#error FRTSYN_SID_SETTRANSMISSIONMODE already defined!
#endif /* #if (defined FRTSYN_SID_SETTRANSMISSIONMODE)*/
#define FRTSYN_SID_SETTRANSMISSIONMODE 0x03U

/** \brief Defines API id of function FrTSyn_TriggerTransmit(). */
#if (defined FRTSYN_SID_TRIGGERTRANSMIT)
#error FRTSYN_SID_TRIGGERTRANSMIT already defined!
#endif /* #if (defined FRTSYN_SID_TRIGGERTRANSMIT)*/
#define FRTSYN_SID_TRIGGERTRANSMIT 0x41U

/** \brief Defines API id of function FrTSyn_RxIndication(). */
#if (defined FRTSYN_SID_RXINDICATION)
#error FRTSYN_SID_RXINDICATION already defined!
#endif /* #if (defined FRTSYN_SID_RXINDICATION)*/
#define FRTSYN_SID_RXINDICATION 0x42U

/** \brief Defines API id of function FrTSyn_MainFunction(). */
#if (defined FRTSYN_SID_MAINFUNCTION)
#error FRTSYN_SID_MAINFUNCTION already defined!
#endif /* #if (defined FRTSYN_SID_MAINFUNCTION)*/
#define FRTSYN_SID_MAINFUNCTION 0x04U

/* === Module Det error codes === */
#if (defined FRTSYN_E_INVALID_PDUID)
#error FRTSYN_E_INVALID_PDUID already defined!
#endif /* #if (defined FRTSYN_E_INVALID_PDUID)*/
#define FRTSYN_E_INVALID_PDUID 0x01U /**< DET error code: Module called with wrong PDU or SDU ID */

#if (defined FRTSYN_E_UNINIT)
#error FRTSYN_E_UNINIT already defined!
#endif /* #if (defined FRTSYN_E_UNINIT)*/
#define FRTSYN_E_UNINIT 0x20U /**< DET error code: API service used in un-initialized state */

#if (defined FRTSYN_E_NULL_POINTER)
#error FRTSYN_E_NULL_POINTER already defined!
#endif /* #if (defined FRTSYN_E_NULL_POINTER)*/
#define FRTSYN_E_NULL_POINTER 0x21U /**< DET error code: Invalid pointer (NULL_PTR) */

#if (defined FRTSYN_E_INIT_FAILED)
#error FRTSYN_E_INIT_FAILED already defined!
#endif /* #if (defined FRTSYN_E_INIT_FAILED)*/
#define FRTSYN_E_INIT_FAILED 0x22U /**< DET error code: FrTSyn initialization failed */

#if (defined FRTSYN_E_PARAM)
#error FRTSYN_E_PARAM already defined!
#endif /* #if (defined FRTSYN_E_PARAM)*/
#define FRTSYN_E_PARAM 0x23U /**< DET error code: API called with invalid parameter */

#if (defined FRTSYN_E_INV_CTRL_IDX)
#error FRTSYN_E_INV_CTRL_IDX already defined!
#endif /* #if (defined FRTSYN_E_INV_CTRL_IDX)*/
#define FRTSYN_E_INV_CTRL_IDX 0x24U /**< DET error code: Invalid Controller index */

#if (defined FRTSYN_IS_MESSAGE_TYPE_VALID)
#error FRTSYN_IS_MESSAGE_TYPE_VALID already defined!
#endif /* #if (defined FRTSYN_IS_MESSAGE_TYPE_VALID)*/
#define FRTSYN_IS_MESSAGE_TYPE_VALID(value, index)    ((((uint8)(value) == (uint8)FrTSyn_SlaveConfig[(index)].rxCrcValidated) \
                                                        && (((uint8)(value) != (uint8)(FRTSYN_CRC_IGNORED)) && ((uint8)(value) != (uint8)(FRTSYN_CRC_OPTIONAL)))) \
                                                   || ((((uint8)(FRTSYN_CRC_IGNORED) == (uint8)FrTSyn_SlaveConfig[(index)].rxCrcValidated)|| ((uint8)(FRTSYN_CRC_OPTIONAL) == (uint8)FrTSyn_SlaveConfig[(index)].rxCrcValidated)) \
                                                      && (((FrTSyn_SlaveConfig[(index)].timeDomainId < (uint8)0x10) && (((uint8)(value) == (uint8)FRTSYN_SYNC_CRC_NOT_VALIDATED) || ((uint8)(value) == (uint8)FRTSYN_SYNC_CRC_VALIDATED)))\
                                                        ||((FrTSyn_SlaveConfig[(index)].timeDomainId > (uint8)0x0F) && (((uint8)(value) == (uint8)FRTSYN_OFS_CRC_NOT_VALIDATED) || ((uint8)(value) == (uint8)FRTSYN_OFS_CRC_VALIDATED))))))

#if (defined FRTSYN_GET_DELTA_DIFF)
#error FRTSYN_GET_DELTA_DIFF already defined!
#endif /* #if (defined FRTSYN_GET_DELTA_DIFF)*/
#define FRTSYN_GET_DELTA_DIFF(delta, deltaMax)  (((delta) > (deltaMax)) ? ((delta) - (deltaMax)) : ((deltaMax) - (delta)))

#if (defined FRTSYN_MAX_SEQ_COUNTER)
#error FRTSYN_MAX_SEQ_COUNTER already defined!
#endif /* #if (defined FRTSYN_MAX_SEQ_COUNTER)*/
#define FRTSYN_MAX_SEQ_COUNTER 15U

#if (defined FRTSYN_GET_NEW_SEQ_COUNTER)
#error FRTSYN_GET_NEW_SEQ_COUNTER already defined!
#endif /* #if (defined FRTSYN_GET_NEW_SEQ_COUNTER)*/
#define FRTSYN_GET_NEW_SEQ_COUNTER(seqCounter)  (((seqCounter) < FRTSYN_MAX_SEQ_COUNTER)?((seqCounter)+1U):(0U))

#if (defined FRTSYN_IS_CRC_SUPPORTED)
#error FRTSYN_IS_CRC_SUPPORTED already defined!
#endif /* #if (defined FRTSYN_IS_CRC_SUPPORTED)*/
#define FRTSYN_IS_CRC_SUPPORTED(index)  ((FrTSyn_MasterConfig[(index)].txCrcSecured == FRTSYN_CRC_SUPPORTED)?TRUE:FALSE)

/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define FRTSYN_START_SEC_CODE
#include <FrTSyn_MemMap.h>

/**
 * \brief Initializes the FrTSyn module.
 *
 * This service initializes the FrTSyn module. It shall be the first function of the module to be
 * called.
 *
 * \param[in] CfgPtr - Address of the configuration data structure.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRTSYN_CODE) FrTSyn_Init
(
  P2CONST(FrTSyn_ConfigType, AUTOMATIC, FRTSYN_APPL_CONST) configPtr
);

#if(FRTSYN_VERSION_INFO_API == STD_ON)
/**
 * \brief API to get the module version information.
 *
 * This service returns the version information of this module.
 *
 * \param[out] versioninfo - Pointer to return the module version information.
 *
 * \ServiceID{0x02}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */

FUNC(void, FRTSYN_CODE) FrTSyn_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, FRTSYN_APPL_DATA) versioninfo
);
#endif /* FRTSYN_VERSION_INFO_API */

/**
 * \brief Initializes the FrTSyn module.
 *
 * This API is used to turn on and off the TX capabilities of the FrTSyn.
 *
 * \param[in] CtrlIdx - Index of the FlexRay channel.
 * \param[in] Mode - FRTSYN_TX_OFF
 *                 - FRTSYN_TX_ON
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRTSYN_CODE) FrTSyn_SetTransmissionMode
(
  uint8 CtrlIdx,
  FrTSyn_TransmissionModeType Mode
);

/**
 * \brief Rx-Indication function
 *
 * This service is called by FrIf in case a reception is indicated.
 *
 * \param[in] RxPduId - ID of the received I-PDU.
 * \param[in] PduInfoPtr - Pointer to a buffer containing the I-PDU.
 *
 * \ServiceID{0x42}
 * \Reentrancy{Reentrant for different PduIds. Non Reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRTSYN_CODE) FrTSyn_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr
);

/**
 * \brief Trigger-Transmit function
 *
 * This service is called by FrIf in case a triggered transmit.
 *
 * \param[in] TxPduId - ID of the received I-PDU.
 * \param[in] PduInfoPtr - Pointer to a buffer containing the I-PDU.
 *
 * \return Result of operation
 * \retval E_OK: SDU has been copied and SduLength indicates the
 * number of copied bytes.
 * \retval E_NOT_OK: No SDU has been copied. PduInfoPtr must not be
 * used since it may contain a NULL pointer or point to invalid data.
 *
 * \ServiceID{0x41}
 * \Reentrancy{Reentrant for different PduIds. Non Reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(Std_ReturnType, FRTSYN_CODE) FrTSyn_TriggerTransmit
(
  PduIdType TxPduId,
  P2VAR(PduInfoType, AUTOMATIC, FRTSYN_APPL_DATA) PduInfoPtr
);

/**
 * \brief TxConfirmation dummy function
 *
 * This service is called by FrIf in case of a Tx Confirmation.
 * But since this is a dummy, this function does not provide any
 * functionality.
 *
 * \param[in] TxPduId - ID of the received I-PDU.
 *
 * \Reentrancy{Reentrant for different PduIds. Non Reentrant for the same PduId.}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRTSYN_CODE) FrTSyn_TxConfirmation
(
  PduIdType TxPduId
);

/**
 * \brief FrTSyn module main function.
 *
 * \ServiceID{0x04}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, FRTSYN_CODE) FrTSyn_MainFunction(void);


#define FRTSYN_STOP_SEC_CODE
#include <FrTSyn_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef FRTSYN_H */
/*==================[end of file]===========================================*/


