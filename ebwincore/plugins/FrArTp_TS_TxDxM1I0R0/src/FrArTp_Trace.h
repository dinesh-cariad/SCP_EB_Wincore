/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRARTP_TRACE_H
#define FRARTP_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <FrArTp_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function FrArTp_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function FrArTp_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRARTP_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_INIT_ENTRY
/** \brief Entry point of function FrArTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_INIT_EXIT
/** \brief Exit point of function FrArTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrArTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrArTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_TRANSMIT_ENTRY
/** \brief Entry point of function FrArTp_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_TRANSMIT_ENTRY(FrArTpTxSduId,FrArTpTxSduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_TRANSMIT_EXIT
/** \brief Exit point of function FrArTp_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRARTP_TRANSMIT_EXIT(Retval,FrArTpTxSduId,FrArTpTxSduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_CANCELTRANSMIT_ENTRY
/** \brief Entry point of function FrArTp_CancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_CANCELTRANSMIT_ENTRY(FrArTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_CANCELTRANSMIT_EXIT
/** \brief Exit point of function FrArTp_CancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRARTP_CANCELTRANSMIT_EXIT(Retval,FrArTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_CANCELRECEIVE_ENTRY
/** \brief Entry point of function FrArTp_CancelReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_CANCELRECEIVE_ENTRY(FrArTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_CANCELRECEIVE_EXIT
/** \brief Exit point of function FrArTp_CancelReceive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRARTP_CANCELRECEIVE_EXIT(Retval,FrArTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function FrArTp_ChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_CHANGEPARAMETER_ENTRY(id,parameter,value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function FrArTp_ChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRARTP_CHANGEPARAMETER_EXIT(Retval,id,parameter,value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_MAINFUNCTION_ENTRY
/** \brief Entry point of function FrArTp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_MAINFUNCTION_EXIT
/** \brief Exit point of function FrArTp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_TXCONFIRMATION_ENTRY
/** \brief Entry point of function FrArTp_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_TXCONFIRMATION_ENTRY(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_TXCONFIRMATION_EXIT
/** \brief Exit point of function FrArTp_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_TXCONFIRMATION_EXIT(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function FrArTp_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_TRIGGERTRANSMIT_ENTRY(TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function FrArTp_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_TRIGGERTRANSMIT_EXIT(TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_RXINDICATION_ENTRY
/** \brief Entry point of function FrArTp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRARTP_RXINDICATION_EXIT
/** \brief Exit point of function FrArTp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRARTP_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
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

#endif /* ifndef FRARTP_TRACE_H */
/*==================[end of file]===========================================*/
