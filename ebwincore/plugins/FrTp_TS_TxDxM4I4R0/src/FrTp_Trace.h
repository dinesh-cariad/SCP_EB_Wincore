/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRTP_TRACE_H
#define FRTP_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

#include <FrTp_guards.h>
#include <FrTp_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function FrTp_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function FrTp_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRTP_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_INIT_ENTRY
/** \brief Entry point of function FrTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_INIT_ENTRY(PBCfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_INIT_EXIT
/** \brief Exit point of function FrTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_INIT_EXIT(PBCfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_TRANSMIT_ENTRY
/** \brief Entry point of function FrTp_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_TRANSMIT_ENTRY(FrTpTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_TRANSMIT_EXIT
/** \brief Exit point of function FrTp_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRTP_TRANSMIT_EXIT(Retval,FrTpTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_CANCELTRANSMIT_ENTRY
/** \brief Entry point of function FrTp_CancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_CANCELTRANSMIT_ENTRY(FrTpTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_CANCELTRANSMIT_EXIT
/** \brief Exit point of function FrTp_CancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRTP_CANCELTRANSMIT_EXIT(Retval,FrTpTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function FrTp_ChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_CHANGEPARAMETER_ENTRY(FrTpTxPduId,parameter,FrTpParameterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function FrTp_ChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRTP_CHANGEPARAMETER_EXIT(Retval,FrTpTxPduId,parameter,FrTpParameterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_CANCELRECEIVE_ENTRY
/** \brief Entry point of function FrTp_CancelReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_CANCELRECEIVE_ENTRY(FrTpRxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_CANCELRECEIVE_EXIT
/** \brief Exit point of function FrTp_CancelReceive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRTP_CANCELRECEIVE_EXIT(Retval,FrTpRxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_MAINFUNCTION_ENTRY
/** \brief Entry point of function FrTp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_MAINFUNCTION_EXIT
/** \brief Exit point of function FrTp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_TXCONFIRMATION_ENTRY
/** \brief Entry point of function FrTp_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_TXCONFIRMATION_ENTRY(FrTxConfirmationPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_TXCONFIRMATION_EXIT
/** \brief Exit point of function FrTp_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_TXCONFIRMATION_EXIT(FrTxConfirmationPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function FrTp_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_TRIGGERTRANSMIT_ENTRY(FrTxConfirmationPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function FrTp_TriggerTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRTP_TRIGGERTRANSMIT_EXIT(Retval,FrTxConfirmationPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_RXINDICATION_ENTRY
/** \brief Entry point of function FrTp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRTP_RXINDICATION_EXIT
/** \brief Exit point of function FrTp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRTP_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
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

#endif /* ifndef FRTP_TRACE_H */
/*==================[end of file]===========================================*/
