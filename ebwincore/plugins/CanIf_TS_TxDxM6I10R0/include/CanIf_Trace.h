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

#ifndef CANIF_TRACE_H
#define CANIF_TRACE_H
/*==================[inclusions]============================================*/
#include <CanIf_Cfg.h>

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <CanIf_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function CanIf_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_ISVALIDCONFIG_EXIT
/** \brief Exit point of function CanIf_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_INIT_ENTRY
/** \brief Entry point of function CanIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_INIT_EXIT
/** \brief Exit point of function CanIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETCONTROLLERMODE_ENTRY
/** \brief Entry point of function CanIf_SetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETCONTROLLERMODE_ENTRY(ControllerId,ControllerMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETCONTROLLERMODE_EXIT
/** \brief Exit point of function CanIf_SetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_SETCONTROLLERMODE_EXIT(Retval,ControllerId,ControllerMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETCONTROLLERMODE_ENTRY
/** \brief Entry point of function CanIf_GetControllerMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETCONTROLLERMODE_ENTRY(ControllerId,ControllerModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETCONTROLLERMODE_EXIT
/** \brief Exit point of function CanIf_GetControllerMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETCONTROLLERMODE_EXIT(Retval,ControllerId,ControllerModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_TRANSMIT_ENTRY
/** \brief Entry point of function CanIf_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_TRANSMIT_ENTRY(CanTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_TRANSMIT_EXIT
/** \brief Exit point of function CanIf_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_TRANSMIT_EXIT(Retval,CanTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETBAUDRATE_ENTRY
/** \brief Entry point of function CanIf_SetBaudrate()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETBAUDRATE_ENTRY(ControllerId,BaudRateConfigID)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETBAUDRATE_EXIT
/** \brief Exit point of function CanIf_SetBaudrate()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_SETBAUDRATE_EXIT(Retval,ControllerId,BaudRateConfigID)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CANCELTRANSMIT_ENTRY
/** \brief Entry point of function CanIf_CancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CANCELTRANSMIT_ENTRY(CanTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CANCELTRANSMIT_EXIT
/** \brief Exit point of function CanIf_CancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_CANCELTRANSMIT_EXIT(Retval,CanTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_READRXPDUDATA_ENTRY
/** \brief Entry point of function CanIf_ReadRxPduData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_READRXPDUDATA_ENTRY(CanRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_READRXPDUDATA_EXIT
/** \brief Exit point of function CanIf_ReadRxPduData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_READRXPDUDATA_EXIT(Retval,CanRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_READTXNOTIFSTATUS_ENTRY
/** \brief Entry point of function CanIf_ReadTxNotifStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_READTXNOTIFSTATUS_ENTRY(CanTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_READTXNOTIFSTATUS_EXIT
/** \brief Exit point of function CanIf_ReadTxNotifStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_READTXNOTIFSTATUS_EXIT(Retval,CanTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_READRXNOTIFSTATUS_ENTRY
/** \brief Entry point of function CanIf_ReadRxNotifStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_READRXNOTIFSTATUS_ENTRY(CanRxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_READRXNOTIFSTATUS_EXIT
/** \brief Exit point of function CanIf_ReadRxNotifStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_READRXNOTIFSTATUS_EXIT(Retval,CanRxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETPDUMODE_ENTRY
/** \brief Entry point of function CanIf_SetPduMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETPDUMODE_ENTRY(ControllerId,PduModeRequest)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETPDUMODE_EXIT
/** \brief Exit point of function CanIf_SetPduMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_SETPDUMODE_EXIT(Retval,ControllerId,PduModeRequest)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETPDUMODE_ENTRY
/** \brief Entry point of function CanIf_GetPduMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETPDUMODE_ENTRY(ControllerId,PduModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETPDUMODE_EXIT
/** \brief Exit point of function CanIf_GetPduMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETPDUMODE_EXIT(Retval,ControllerId,PduModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETVERSIONINFO_ENTRY
/** \brief Entry point of function CanIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETVERSIONINFO_ENTRY(Versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETVERSIONINFO_EXIT
/** \brief Exit point of function CanIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETVERSIONINFO_EXIT(Versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETDYNAMICTXID_ENTRY
/** \brief Entry point of function CanIf_SetDynamicTxId()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETDYNAMICTXID_ENTRY(CanTxPduId,CanId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETDYNAMICTXID_EXIT
/** \brief Exit point of function CanIf_SetDynamicTxId()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETDYNAMICTXID_EXIT(CanTxPduId,CanId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETTRCVMODE_ENTRY
/** \brief Entry point of function CanIf_SetTrcvMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETTRCVMODE_ENTRY(TransceiverId,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETTRCVMODE_EXIT
/** \brief Exit point of function CanIf_SetTrcvMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_SETTRCVMODE_EXIT(Retval,TransceiverId,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)
/* This wouldn't be necessary since the values are just passed on */
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTRCVMODE_ENTRY
/** \brief Entry point of function CanIf_GetTrcvMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETTRCVMODE_ENTRY(TransceiverId,TransceiverModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTRCVMODE_EXIT
/** \brief Exit point of function CanIf_GetTrcvMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETTRCVMODE_EXIT(Retval,TransceiverId,TransceiverModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#else
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTRCVMODE_ENTRY
/** \brief Entry point of function CanIf_GetTrcvMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETTRCVMODE_ENTRY(TransceiverModePtr,TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTRCVMODE_EXIT
/** \brief Exit point of function CanIf_GetTrcvMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETTRCVMODE_EXIT(Retval,TransceiverModePtr,TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTRCVWAKEUPREASON_ENTRY
/** \brief Entry point of function CanIf_GetTrcvWakeupReason()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETTRCVWAKEUPREASON_ENTRY(TransceiverId,TrcvWuReasonPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTRCVWAKEUPREASON_EXIT
/** \brief Exit point of function CanIf_GetTrcvWakeupReason()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETTRCVWAKEUPREASON_EXIT(Retval,TransceiverId,TrcvWuReasonPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETTRCVWAKEUPMODE_ENTRY
/** \brief Entry point of function CanIf_SetTrcvWakeupMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_SETTRCVWAKEUPMODE_ENTRY(TransceiverId,TrcvWakeupMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_SETTRCVWAKEUPMODE_EXIT
/** \brief Exit point of function CanIf_SetTrcvWakeupMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_SETTRCVWAKEUPMODE_EXIT(Retval,TransceiverId,TrcvWakeupMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKWAKEUP_ENTRY
/** \brief Entry point of function CanIf_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CHECKWAKEUP_ENTRY(WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKWAKEUP_EXIT
/** \brief Exit point of function CanIf_CheckWakeup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_CHECKWAKEUP_EXIT(Retval,WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKVALIDATION_ENTRY
/** \brief Entry point of function CanIf_CheckValidation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CHECKVALIDATION_ENTRY(WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKVALIDATION_EXIT
/** \brief Exit point of function CanIf_CheckValidation()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_CHECKVALIDATION_EXIT(Retval,WakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTXCONFIRMATIONSTATE_ENTRY
/** \brief Entry point of function CanIf_GetTxConfirmationState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETTXCONFIRMATIONSTATE_ENTRY(ControllerId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETTXCONFIRMATIONSTATE_EXIT
/** \brief Exit point of function CanIf_GetTxConfirmationState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETTXCONFIRMATIONSTATE_EXIT(Retval,ControllerId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CLEARTRCVWUFFLAG_ENTRY
/** \brief Entry point of function CanIf_ClearTrcvWufFlag()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CLEARTRCVWUFFLAG_ENTRY(TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CLEARTRCVWUFFLAG_EXIT
/** \brief Exit point of function CanIf_ClearTrcvWufFlag()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_CLEARTRCVWUFFLAG_EXIT(Retval,TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKTRCVWAKEFLAG_ENTRY
/** \brief Entry point of function CanIf_CheckTrcvWakeFlag()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CHECKTRCVWAKEFLAG_ENTRY(TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKTRCVWAKEFLAG_EXIT
/** \brief Exit point of function CanIf_CheckTrcvWakeFlag()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_CHECKTRCVWAKEFLAG_EXIT(Retval,TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_TXCONFIRMATION_ENTRY
/** \brief Entry point of function CanIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_TXCONFIRMATION_ENTRY(CanTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_TXCONFIRMATION_EXIT
/** \brief Exit point of function CanIf_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_TXCONFIRMATION_EXIT(CanTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


#if (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY)
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_RXINDICATION_ENTRY
/** \brief Entry point of function CanIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_RXINDICATION_ENTRY(Hrh,CanId,CanDlc,CanSduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_RXINDICATION_EXIT
/** \brief Exit point of function CanIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_RXINDICATION_EXIT(Hrh,CanId,CanDlc,CanSduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#else
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_RXINDICATION_ENTRY
/** \brief Entry point of function CanIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_RXINDICATION_ENTRY(Mailbox,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_RXINDICATION_EXIT
/** \brief Exit point of function CanIf_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_RXINDICATION_EXIT(Mailbox,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#endif /* (defined CANIF_CAN_ASR402_COMPATIBILITY) || (defined CANIF_CAN_ASR403_COMPATIBILITY) */


#ifndef CANIF_CAN_ASR402_COMPATIBILITY
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CANCELTXCONFIRMATION_ENTRY
/** \brief Entry point of function CanIf_CancelTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CANCELTXCONFIRMATION_ENTRY(CanTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CANCELTXCONFIRMATION_EXIT
/** \brief Exit point of function CanIf_CancelTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CANCELTXCONFIRMATION_EXIT(CanTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */
#endif /* ifndef CANIF_CAN_ASR402_COMPATIBILITY */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CONTROLLERBUSOFF_ENTRY
/** \brief Entry point of function CanIf_ControllerBusOff()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CONTROLLERBUSOFF_ENTRY(Controller)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CONTROLLERBUSOFF_EXIT
/** \brief Exit point of function CanIf_ControllerBusOff()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CONTROLLERBUSOFF_EXIT(Controller)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CONTROLLERMODEINDICATION_ENTRY
/** \brief Entry point of function CanIf_ControllerModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CONTROLLERMODEINDICATION_ENTRY(Controller,ControllerMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CONTROLLERMODEINDICATION_EXIT
/** \brief Exit point of function CanIf_ControllerModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CONTROLLERMODEINDICATION_EXIT(Controller,ControllerMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_TRCVMODEINDICATION_ENTRY
/** \brief Entry point of function CanIf_TrcvModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_TRCVMODEINDICATION_ENTRY(Transceiver,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_TRCVMODEINDICATION_EXIT
/** \brief Exit point of function CanIf_TrcvModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_TRCVMODEINDICATION_EXIT(Transceiver,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CONFIRMPNAVAILABILITY_ENTRY
/** \brief Entry point of function CanIf_ConfirmPnAvailability()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CONFIRMPNAVAILABILITY_ENTRY(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CONFIRMPNAVAILABILITY_EXIT
/** \brief Exit point of function CanIf_ConfirmPnAvailability()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CONFIRMPNAVAILABILITY_EXIT(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CLEARTRCVWUFFLAGINDICATION_ENTRY
/** \brief Entry point of function CanIf_ClearTrcvWufFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CLEARTRCVWUFFLAGINDICATION_ENTRY(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CLEARTRCVWUFFLAGINDICATION_EXIT
/** \brief Exit point of function CanIf_ClearTrcvWufFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CLEARTRCVWUFFLAGINDICATION_EXIT(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKTRCVWAKEFLAGINDICATION_ENTRY
/** \brief Entry point of function CanIf_CheckTrcvWakeFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CHECKTRCVWAKEFLAGINDICATION_ENTRY(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_CHECKTRCVWAKEFLAGINDICATION_EXIT
/** \brief Exit point of function CanIf_CheckTrcvWakeFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_CHECKTRCVWAKEFLAGINDICATION_EXIT(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

#if (CANIF_BUS_MIRRORING_SUPPORT == STD_ON)

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_ENABLEBUSMIRRORING_ENTRY
/** \brief Entry point of function CanIf_EnableBusMirroring()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_ENABLEBUSMIRRORING_ENTRY(ControllerId,MirroringActive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_ENABLEBUSMIRRORING_EXIT
/** \brief Exit point of function CanIf_EnableBusMirroring()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_ENABLEBUSMIRRORING_EXIT(Retval,ControllerId,MirroringActive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETCONTROLLERERRORSTATE_ENTRY
/** \brief Entry point of function CanIf_GetControllerErrorState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETCONTROLLERERRORSTATE_ENTRY(ControllerId,ErrorStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETCONTROLLERERRORSTATE_EXIT
/** \brief Exit point of function CanIf_GetControllerErrorState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETCONTROLLERERRORSTATE_EXIT(Retval,ControllerId,ErrorStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETCONTROLLERTXERRORCOUNTER_ENTRY
/** \brief Entry point of function CanIf_GetControllerTxErrorCounter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANIF_GETCONTROLLERTXERRORCOUNTER_ENTRY(ControllerId,TxErrorCounterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANIF_GETCONTROLLERTXERRORCOUNTER_EXIT
/** \brief Exit point of function CanIf_GetControllerTxErrorCounter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANIF_GETCONTROLLERTXERRORCOUNTER_EXIT(Retval,ControllerId,TxErrorCounterPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

#endif /* CANIF_BUS_MIRRORING_SUPPORT == STD_ON */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef CANIF_TRACE_H */
/*==================[end of file]===========================================*/
