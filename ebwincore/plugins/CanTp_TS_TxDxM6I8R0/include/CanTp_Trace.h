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

#ifndef CANTP_TRACE_H
#define CANTP_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <CanTp_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function CanTp_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function CanTp_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_INIT_ENTRY
/** \brief Entry point of function CanTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_INIT_ENTRY(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_INIT_EXIT
/** \brief Exit point of function CanTp_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_INIT_EXIT(CfgPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_MAINFUNCTION_ENTRY
/** \brief Entry point of function CanTp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_MAINFUNCTION_EXIT
/** \brief Exit point of function CanTp_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_TRANSMIT_ENTRY
/** \brief Entry point of function CanTp_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_TRANSMIT_ENTRY(CanTpTxSduId,CanTpTxInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_TRANSMIT_EXIT
/** \brief Exit point of function CanTp_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_TRANSMIT_EXIT(Retval,CanTpTxSduId,CanTpTxInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function CanTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_GETVERSIONINFO_ENTRY(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_GETVERSIONINFO_EXIT
/** \brief Exit point of function CanTp_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_GETVERSIONINFO_EXIT(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CANCELTRANSMIT_ENTRY
/** \brief Entry point of function CanTp_CancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_CANCELTRANSMIT_ENTRY(CanTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CANCELTRANSMIT_EXIT
/** \brief Exit point of function CanTp_CancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_CANCELTRANSMIT_EXIT(Retval,CanTpTxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_SETNSA_ENTRY
/** \brief Entry point of function CanTp_SetNSa()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_SETNSA_ENTRY(CanTpPduId,CanTpDirection,CanTpNSa)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_SETNSA_EXIT
/** \brief Exit point of function CanTp_SetNSa()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_SETNSA_EXIT(Retval,CanTpPduId,CanTpDirection,CanTpNSa)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_GETNSA_ENTRY
/** \brief Entry point of function CanTp_GetNSa()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_GETNSA_ENTRY(CanTpPduId,CanTpDirection,CanTpNSaPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_GETNSA_EXIT
/** \brief Exit point of function CanTp_GetNSa()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_GETNSA_EXIT(Retval,CanTpPduId,CanTpDirection,CanTpNSaPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CHANGEPARAMETER_ENTRY
/** \brief Entry point of function CanTp_ChangeParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_CHANGEPARAMETER_ENTRY(Id,Parameter,Value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CHANGEPARAMETER_EXIT
/** \brief Exit point of function CanTp_ChangeParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_CHANGEPARAMETER_EXIT(Retval,Id,Parameter,Value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CHANGETXPARAMETER_ENTRY
/** \brief Entry point of function CanTp_ChangeTxParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_CHANGETXPARAMETER_ENTRY(Id,Parameter,Value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CHANGETXPARAMETER_EXIT
/** \brief Exit point of function CanTp_ChangeTxParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_CHANGETXPARAMETER_EXIT(Retval,Id,Parameter,Value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_RESETTXPARAMETER_ENTRY
/** \brief Entry point of function CanTp_ResetTxParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_RESETTXPARAMETER_ENTRY(Id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_RESETTXPARAMETER_EXIT
/** \brief Exit point of function CanTp_ResetTxParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_RESETTXPARAMETER_EXIT(Id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_READPARAMETER_ENTRY
/** \brief Entry point of function CanTp_ReadParameter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_READPARAMETER_ENTRY(Id,Parameter,Value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_READPARAMETER_EXIT
/** \brief Exit point of function CanTp_ReadParameter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_READPARAMETER_EXIT(Retval,Id,Parameter,Value)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CANCELRECEIVE_ENTRY
/** \brief Entry point of function CanTp_CancelReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_CANCELRECEIVE_ENTRY(CanTpRxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_CANCELRECEIVE_EXIT
/** \brief Exit point of function CanTp_CancelReceive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_CANTP_CANCELRECEIVE_EXIT(Retval,CanTpRxSduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_RXINDICATION_ENTRY
/** \brief Entry point of function CanTp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_RXINDICATION_ENTRY(CanTpRxPduId,CanTpRxPduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_RXINDICATION_EXIT
/** \brief Exit point of function CanTp_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_RXINDICATION_EXIT(CanTpRxPduId,CanTpRxPduPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_TXCONFIRMATION_ENTRY
/** \brief Entry point of function CanTp_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_TXCONFIRMATION_ENTRY(CanTpTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_CANTP_TXCONFIRMATION_EXIT
/** \brief Exit point of function CanTp_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_CANTP_TXCONFIRMATION_EXIT(CanTpTxPduId)
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

#endif /* ifndef CANTP_TRACE_H */
/*==================[end of file]===========================================*/
