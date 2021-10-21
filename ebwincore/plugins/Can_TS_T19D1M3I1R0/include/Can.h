/**
 * \file
 *
 * \brief AUTOSAR Can
 *
 * This file contains the implementation of the AUTOSAR
 * module Can.
 *
 * \version 3.1.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
#ifndef CAN_H
#define CAN_H

/*
 * Misra-C:2004 Deviations:
 *
 * MISRA-1) Deviated Rule: 1.4 (required)
 *   The compiler/linker shall be checked to ensure that 31 character
 *   significance and case sensitivity are supported for external identifiers.
 *
 *   Reason:
 *   WinCore gcc is safe
 *
 * MISRA-2) Deviated Rule: 5.1 (required)
 *   Identifiers (internal and external) shall not rely on the significance
 *   of more then 31 characters.
 *
 *   Reason:
 *   Identifiers differ within the first 31 characters.
 *
 * MISRA-3) Deviated Rule: 19.1 (advisory)
 *   '#include' statements in a file should only be preceded by other
 *   preprocessor directives or comments.
 *
 *   Reason:
 *   Inclusion of MemMap.h.
 *
 */

/*==================[inclusions]============================================*/
#include <ComStack_Types.h>
#include <Can_Version.h>
#include <Can_GeneralTypes.h>
#include <Can_Cfg.h>
#include <Can_Internal.h>
#include <VirtualBusInterfaceC.h>

/*==================[macros]================================================*/
/** \brief DET error code CAN_E_PARAM_POINTER */
#define CAN_E_PARAM_POINTER                   0x01U

/** \brief DET error code CAN_E_PARAM_HANDLE */
#define CAN_E_PARAM_HANDLE                    0x02U

/** \brief DET error code CAN_E_PARAM_DLC */
#define CAN_E_PARAM_DLC                       0x03U

/** \brief DET error code CAN_E_PARAM_CONTROLLER */
#define CAN_E_PARAM_CONTROLLER                0x04U

/** \brief DET error code CAN_E_UNINIT */
#define CAN_E_UNINIT                          0x05U

/** \brief DET error code CAN_E_TRANSITION */
#define CAN_E_TRANSITION                      0x06U

/** \brief DET error code CAN_E_DATALOST*/
#define CAN_E_DATALOST                        0x07U

/** \brief Service ID of Can_Init() */
#define CAN_API_INIT                          0x00U

/** \brief Service ID of Can_Init() */
#define CAN_API_GET_VERSION_INFO              0x07U

/** \brief Service ID of Can_Init() */
#define CAN_API_INIT_CONTROLLER               0x02U

/** \brief Service ID of Can_Init() */
#define CAN_API_SET_CONTROLLER_MODE           0x03U

/** \brief Service ID of Can_Init() */
/* Deviation MISRA-1 */
/* Deviation MISRA-2 */
#define CAN_API_DISABLE_CONTROLLER_INTERRUPTS 0x04U

/** \brief Service ID of Can_Init() */
/* Deviation MISRA-1 */
/* Deviation MISRA-2 */
#define CAN_API_ENABLE_CONTROLLER_INTERRUPTS  0x05U

/** \brief Service ID of Can_Init() */
#define CAN_API_CBK_CHECK_WAKEUP              0x0bU

/** \brief Service ID of Can_Init() */
#define CAN_API_WRITE                         0x06U

/** \brief Service ID of Can_Init() */
#define CAN_API_MAIN_FUNCTION_WRITE           0x01U

/** \brief Service ID of Can_Init() */
#define CAN_API_MAIN_FUNCTION_READ            0x08U

/** \brief Service ID of Can_MainFunction_Busoff() */
#define CAN_API_MAIN_FUNCTION_BUS_OFF         0x09U

/** \brief Service ID of Can_MainFunction_Wakeup() */
#define CAN_API_MAIN_FUNCTION_WAKEUP          0x0aU

/** \brief Service ID of Can_MainFunction_Mode() */
#define CAN_API_MAIN_FUNCTION_MODE            0x0cU

/** \brief Service ID of Can_StoreInRxBuffer() */
#define CAN_API_STORE_IN_RX_BUFFER            0x10U

/** \brief Service ID of Can_StoreInTxBuffer() */
#define CAN_API_STORE_IN_TX_BUFFER            0x11U

/*==================[type definitions]======================================*/
/** \brief Overall initialization data for all controllers
 **
 ** This is the type of the external data structure containing the overall
 ** initialization data for the CAN driver and SFR settings affecting all
 ** controllers. Furthermore it contains pointers to controller structures.
 ** The contents of the initialization data structure are CAN hardware
 ** specific.
 */

typedef struct
{
  uint8 dummy;
} Can_ConfigType;

/* Deviation MISRA-1 */
/* Deviation MISRA-2 */
typedef Can_ControllerType Can_ControllerBaudrateConfigType;

/*==================[external function declarations]========================*/
/* Deviation MISRA-3 */
#define CAN_START_SEC_CODE
#include <Can_MemMap.h>

/** \brief Init
 **
 ** This function initialized the module.
 **
 ** \param[in] Config Pointer to driver configuration.
 **
 ** \ServiceID{0x00}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CAN_CODE) Can_Init
(
  P2CONST(Can_ConfigType, AUTOMATIC, CAN_CONST) Config
);

#if (CAN_GET_VERSION_INFO_API == STD_ON)
/** \brief Get version info
 **
 ** This function returns the version information of this module.
 **
 ** \param[out] versioninfo Pointer to where to store the version
 **                         information of this module.
 **
 ** \ServiceID{0x07}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CAN_CODE) Can_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, CAN_APPL_DATA) versioninfo
);
#endif /* (CAN_GET_VERSION_INFO_API == STD_ON) */

/** \brief Init controller
 **
 ** This function initializes only CAN controller specific settings.
 **
 ** \param[in] Controller CAN controller to be initialized
 ** \param[in] Config Pointer to controller configuration
 **
 ** \ServiceID{0x02}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CAN_CODE) Can_InitController
(
  uint8 Controller,
  P2CONST(Can_ControllerBaudrateConfigType, AUTOMATIC, CAN_CONST) Config
);

/** \brief Set controller mode
 **
 ** This function performs software triggered state transitions of
 ** the CAN controller state machine.
 **
 ** \param[in] Controller CAN controller for which the status shall
 **                       be changed
 ** \param[in] Transition State transition to be performed
 **
 ** \param[out]
 **
 ** \return Status of the state change
 ** \retval CAN_OK transition initiated
 ** \retval CAN_NOT_OK Development or production or a wakeup during
 **                    transition to 'sleep' occured.
 **
 ** \ServiceID{0x03}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Asynchronous}
 */
extern FUNC(Can_ReturnType, CAN_CODE) Can_SetControllerMode
(
  uint8 Controller,
  Can_StateTransitionType Transition
);

/** \brief Disable controller interrupts
 **
 ** This function disables all interrupts for this CAN controller
 **
 ** \param[in] Controller CAN controller for which interrupts shall
 **                       be disabled.
 **
 ** \ServiceID{0x04}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CAN_CODE) Can_DisableControllerInterrupts
(
  uint8 Controller
);

/** \brief Enable controller interrupts
 **
 ** This function enables all allowed interrupts.
 **
 ** \param[in] Controlelr CAN controller for which interrupts shall
 **                       be re-enabled.
 **
 ** \ServiceID{0x05}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(void, CAN_CODE) Can_EnableControllerInterrupts
(
  uint8 Controller
);

/** \brief Check wakeup callback
 **
 ** This function checks if a wakeup has occured for the given controller.
 **
 ** \param[in] Controller Controller to be checked for a wakeup
 **
 ** \return Result of the check for a wakeup
 ** \retval E_OK A wakeup was detected for given controller.
 ** \retval E_NOT_OK No wakeup was detected for the given controller.
 **
 ** \ServiceID{0x0b}
 ** \Reentrancy{Non-Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Can_ReturnType, CAN_CODE) Can_CheckWakeup
(
  uint8 Controller
);

/** \brief Write
 **
 ** This function triggers the transmission of a CAN frame.
 **
 ** \param[in] Hth Information which HW-transmit handle shall be used
 **                for transmit. Implicitly this is also the information
 **                about the controller to use because the Hth numbers
 **                are unique inside one hardware unit.
 **
 **
 ** \param[in] PduInfo Pointer to SDU user memory, DLC and identifier.
 **
 ** \return Success of transmission request
 ** \retval CAN_OK Write command has been accepted.
 ** \retval CAN_NOT_OK Development error occured.
 ** \retval CAN_BUSY No TX hardware buffer available or preemtive call
 **                  of Can_Write that can't be implemented reentrant.
 **
 ** \ServiceID{0x06}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 */
extern FUNC(Can_ReturnType, CAN_CODE) Can_Write
(
  uint8 Hth,
  P2CONST(Can_PduType, AUTOMATIC, CAN_APPL_CONST) PduInfo
);

extern FUNC(void, CAN_CODE) Can_MainFunction_Write_Internal(void);

extern FUNC(void, CAN_CODE) Can_MainFunction_Read_Internal(void);

/* Deviation MISRA-3 */
#define CAN_STOP_SEC_CODE
#include <Can_MemMap.h>
/*==================[external constants]====================================*/
/* Deviation MISRA-3 */
#define CAN_START_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>

/** \brief Can Config
 **
 ** This is the dummy constant to be used as parameter for Can_Init();
 */
extern CONST(Can_ConfigType, CAN_CONST) CAN_CONFIG_SET;

/** \brief Can Controller Config
 **
 ** This is the dummy constant to be used as parameter for Can_InitController();
 */
extern CONST(Can_ControllerBaudrateConfigType, CAN_CONST) Can_ControllerConfigDummy;

/* Deviation MISRA-3 */
#define CAN_STOP_SEC_CONST_UNSPECIFIED
#include <Can_MemMap.h>
/*==================[external data]=========================================*/

#endif /* CAN_H */
