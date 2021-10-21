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

#ifndef CANSM_TRACE_H
#define CANSM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <CanSM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function CanSM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_GETVERSIONINFO_ENTRY(VersionInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_GETVERSIONINFO_EXIT
/** \brief Exit point of function CanSM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_GETVERSIONINFO_EXIT(VersionInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_INIT_ENTRY
/** \brief Entry point of function CanSM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_INIT_EXIT
/** \brief Exit point of function CanSM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CONTROLLERBUSOFF_ENTRY
/** \brief Entry point of function CanSM_ControllerBusOff()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CONTROLLERBUSOFF_ENTRY(ControllerId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CONTROLLERBUSOFF_EXIT
/** \brief Exit point of function CanSM_ControllerBusOff()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CONTROLLERBUSOFF_EXIT(ControllerId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CONTROLLERMODEINDICATION_ENTRY
/** \brief Entry point of function CanSM_ControllerModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CONTROLLERMODEINDICATION_ENTRY(ControllerId,ControllerMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CONTROLLERMODEINDICATION_EXIT
/** \brief Exit point of function CanSM_ControllerModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CONTROLLERMODEINDICATION_EXIT(ControllerId,ControllerMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_TRANSCEIVERMODEINDICATION_ENTRY
/** \brief Entry point of function CanSM_TransceiverModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_TRANSCEIVERMODEINDICATION_ENTRY(TransceiverId,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_TRANSCEIVERMODEINDICATION_EXIT
/** \brief Exit point of function CanSM_TransceiverModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_TRANSCEIVERMODEINDICATION_EXIT(TransceiverId,TransceiverMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CLEARTRCVWUFFLAGINDICATION_ENTRY
/** \brief Entry point of function CanSM_ClearTrcvWufFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CLEARTRCVWUFFLAGINDICATION_ENTRY(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CLEARTRCVWUFFLAGINDICATION_EXIT
/** \brief Exit point of function CanSM_ClearTrcvWufFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CLEARTRCVWUFFLAGINDICATION_EXIT(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_ENTRY
/** \brief Entry point of function CanSM_CheckTransceiverWakeFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_ENTRY(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_EXIT
/** \brief Exit point of function CanSM_CheckTransceiverWakeFlagIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CHECKTRANSCEIVERWAKEFLAGINDICATION_EXIT(Transceiver)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CONFIRMPNAVAILABILITY_ENTRY
/** \brief Entry point of function CanSM_ConfirmPnAvailability()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CONFIRMPNAVAILABILITY_ENTRY(TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_CONFIRMPNAVAILABILITY_EXIT
/** \brief Exit point of function CanSM_ConfirmPnAvailability()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_CONFIRMPNAVAILABILITY_EXIT(TransceiverId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function CanSM_RequestComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_REQUESTCOMMODE_ENTRY(network,ComM_Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_REQUESTCOMMODE_EXIT
/** \brief Exit point of function CanSM_RequestComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANSM_REQUESTCOMMODE_EXIT(Retval,network,ComM_Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_GETCURRENTCOMMODE_ENTRY
/** \brief Entry point of function CanSM_GetCurrentComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_GETCURRENTCOMMODE_ENTRY(network,ComM_ModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_GETCURRENTCOMMODE_EXIT
/** \brief Exit point of function CanSM_GetCurrentComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANSM_GETCURRENTCOMMODE_EXIT(Retval,network,ComM_ModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_SETBAUDRATE_ENTRY
/** \brief Entry point of function CanSM_SetBaudrate()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_SETBAUDRATE_ENTRY(network,BaudRateConfigID)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_SETBAUDRATE_EXIT
/** \brief Exit point of function CanSM_SetBaudrate()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANSM_SETBAUDRATE_EXIT(Retval,network,BaudRateConfigID)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_MAINFUNCTION_ENTRY
/** \brief Entry point of function CanSM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_MAINFUNCTION_EXIT
/** \brief Exit point of function CanSM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_TXTIMEOUTEXCEPTION_ENTRY
/** \brief Entry point of function CanSM_TxTimeoutException()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_TXTIMEOUTEXCEPTION_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANSM_TXTIMEOUTEXCEPTION_EXIT
/** \brief Exit point of function CanSM_TxTimeoutException()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANSM_TXTIMEOUTEXCEPTION_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[internal function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[internal constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal data]=========================================*/

/*==================[external function definitions]=========================*/

/*==================[internal function definitions]=========================*/

#endif /* ifndef CANSM_TRACE_H */
/*==================[end of file]===========================================*/


