/**
 * \file
 *
 * \brief AUTOSAR CanTSyn
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTSyn.
 *
 * \version 2.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef CANTSYN_H
#define CANTSYN_H

/*==================[inclusions]============================================*/
#include <ComStack_Types.h> /* AUTOSAR Communication Stack types */
#include <CanTSyn_Cfg.h> /* Generated configuration */
#include <CanTSyn_Version.h> /* Module version declarations */
/*==================[macros]================================================*/


#if (defined CANTSYN_INSTANCE_ID)
#error CANTSYN_INSTANCE_ID is already defined
#endif /* if (defined CANTSYN_INSTANCE_ID) */

/** \brief Module instance ID
 *
 * Defines the instance number of this module. Since multiple instances are not supported this ID
 * is always zero.
 **/
#define CANTSYN_INSTANCE_ID 0U

#if (defined CANTSYN_SID_INIT)
#error CANTSYN_SID_INIT is already defined
#endif /* if (defined CANTSYN_SID_INIT) */

/** \brief Defines API id of function CanTSyn_Init(). */
#define CANTSYN_SID_INIT 0x01U

#if (defined CANTSYN_SID_GETVERSIONINFO)
#error CANTSYN_SID_GETVERSIONINFO is already defined
#endif /* if (defined CANTSYN_SID_GETVERSIONINFO) */

/** \brief Defines API id of function CanTSyn_GetVersionInfo(). */
#define CANTSYN_SID_GETVERSIONINFO 0x02U

#if (defined CANTSYN_SID_SETTRANSMISSIONMODE)
#error CANTSYN_SID_SETTRANSMISSIONMODE is already defined
#endif /* if (defined CANTSYN_SID_SETTRANSMISSIONMODE) */

/** \brief Defines API id of function CanTSyn_SetCurrentTime(). */
#define CANTSYN_SID_SETTRANSMISSIONMODE 0x03U

#if (defined CANTSYN_SID_RXINDICATION)
#error CANTSYN_SID_RXINDICATION is already defined
#endif /* if (defined CANTSYN_SID_RXINDICATION) */

/** \brief Defines API id of function CanTSyn_RxIndication(). */
#define CANTSYN_SID_RXINDICATION 0x42U

#if (defined CANTSYN_SID_TXCONFIRMATION)
#error CANTSYN_SID_TXCONFIRMATION is already defined
#endif /* if (defined CANTSYN_SID_TXCONFIRMATION) */

/** \brief Defines API id of function CanTSyn_TxConfirmation(). */
#define CANTSYN_SID_TXCONFIRMATION 0x40U

#if (defined CANTSYN_SID_MAINFUNCTION)
#error CANTSYN_SID_MAINFUNCTION is already defined
#endif /* if (defined CANTSYN_SID_MAINFUNCTION) */

/** \brief Defines API id of function CanTSyn_MainFunction(). */
#define CANTSYN_SID_MAINFUNCTION 0x06U

/* === Module Det error codes === */
#if (defined CANTSYN_E_INVALID_PDUID)
#error CANTSYN_E_INVALID_PDUID is already defined
#endif /* if (defined CANTSYN_E_INVALID_PDUID) */

#define CANTSYN_E_INVALID_PDUID 0x01U /**< DET error code: Module called with wrong PDU or SDU ID */

#if (defined CANTSYN_E_UNINIT)
#error CANTSYN_E_UNINIT is already defined
#endif /* if (defined CANTSYN_E_UNINIT) */

#define CANTSYN_E_UNINIT 0x02U /**< DET error code: Module not initialized */

#if (defined CANTSYN_E_NULL_POINTER)
#error CANTSYN_E_NULL_POINTER is already defined
#endif /* if (defined CANTSYN_E_NULL_POINTER) */

#define CANTSYN_E_NULL_POINTER 0x03U /**< DET error code: Invalid pointer (NULL_PTR) */

#if (defined CANTSYN_E_INIT_FAILED)
#error CANTSYN_E_INIT_FAILED is already defined
#endif /* if (defined CANTSYN_E_INIT_FAILED) */

#define CANTSYN_E_INIT_FAILED 0x04U /**< DET error code: CanTSyn initialization failed */

#if (defined CANTSYN_E_PARAM)
#error CANTSYN_E_PARAM is already defined
#endif /* if (defined CANTSYN_E_PARAM) */

#define CANTSYN_E_PARAM 0x05U /**< DET error code: API called with invalid parameter */

#if (defined CANTSYN_E_INV_CTRL_IDX)
#error CANTSYN_E_INV_CTRL_IDX is already defined
#endif /* if (defined CANTSYN_E_INV_CTRL_IDX) */

#define CANTSYN_E_INV_CTRL_IDX 0x06U /**< DET error code: Invalid Controller index */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

#define CANTSYN_START_SEC_CODE
#include <CanTSyn_MemMap.h>

/**
 * \brief Initializes the CanTSyn module.
 *
 * This service initializes the CanTSyn module. It shall be the first function of the module to be
 * called.
 *
 * \param[in] configPtr - Address of the configuration data structure.
 *
 * \ServiceID{0x01}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTSYN_CODE) CanTSyn_Init
(
  P2CONST(CanTSyn_ConfigType, AUTOMATIC, CANTSYN_APPL_CONST) configPtr
);

#if(CANTSYN_VERSION_INFO_API == STD_ON)
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

FUNC(void, CANTSYN_CODE) CanTSyn_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CANTSYN_APPL_DATA) versioninfo
);
#endif /* CANTSYN_VERSION_INFO_API */


#if((CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON))
/**
 * \brief This service is used to turn on or off the TX capabilities of CanTSyn.
 *
 * \param[in] CtrlIdx - Index of the CAN channel.
 * \param[in] Mode - Mode to indicate if frame shall be transmitted.
 *
 * \ServiceID{0x03}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTSYN_CODE) CanTSyn_SetTransmissionMode
(
  uint8 CtrlIdx,
  CanTSyn_TransmissionModeType Mode
);
#endif /* (CANTSYN_TX_SYNC_USED == STD_ON) || (CANTSYN_TX_OFFSET_USED == STD_ON) */

/**
 * \brief Rx-Indication function
 *
 * This service is called by CanIf in case a reception is indicated.
 *
 * \param[in] RxPduId - ID of the received I-PDU.
 * \param[in] PduInfoPtr - Pointer to a buffer containing the I-PDU.
 *
 * \ServiceID{0x42}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTSYN_CODE) CanTSyn_RxIndication
(
  PduIdType RxPduId,
  P2VAR(PduInfoType, AUTOMATIC, CANTSYN_APPL_DATA) PduInfoPtr
);

/**
 * \brief Tx-Confirmation callback function.
 *
 * This service is called by CanIf in case a transmission is confirmed.
 *
 * \param[in] TxPduId - ID of the I-PDU that has been transmitted.
 *
 * \ServiceID{0x07}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTSYN_CODE) CanTSyn_TxConfirmation
(
  PduIdType TxPduId
);

/**
 * \brief CanTSyn module main function.
 *
 * \ServiceID{0x06}
 * \Reentrancy{Non Reentrant}
 * \Synchronicity{Synchronous}
 */
extern FUNC(void, CANTSYN_CODE) CanTSyn_MainFunction(void);


#define CANTSYN_STOP_SEC_CODE
#include <CanTSyn_MemMap.h>
/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/


#endif /* ifndef CANTSYN_H */
/*==================[end of file]===========================================*/
