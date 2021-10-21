/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef IPDUM_TRACE_H
#define IPDUM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <IpduM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_INIT_ENTRY
/** \brief Entry point of function IpduM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_INIT_ENTRY(Config)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_INIT_EXIT
/** \brief Exit point of function IpduM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_INIT_EXIT(Config)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_TRANSMIT_ENTRY
/** \brief Entry point of function IpduM_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_TRANSMIT_ENTRY(TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_TRANSMIT_EXIT
/** \brief Exit point of function IpduM_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_IPDUM_TRANSMIT_EXIT(Retval,TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_MAINFUNCTIONRX_ENTRY
/** \brief Entry point of function IpduM_MainFunctionRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_MAINFUNCTIONRX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_MAINFUNCTIONRX_EXIT
/** \brief Exit point of function IpduM_MainFunctionRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_MAINFUNCTIONRX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function IpduM_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_MAINFUNCTIONTX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function IpduM_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_MAINFUNCTIONTX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function IpduM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_GETVERSIONINFO_EXIT
/** \brief Exit point of function IpduM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_RXINDICATION_ENTRY
/** \brief Entry point of function IpduM_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_RXINDICATION_EXIT
/** \brief Exit point of function IpduM_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function IpduM_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_TRIGGERTRANSMIT_ENTRY(TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function IpduM_TriggerTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_IPDUM_TRIGGERTRANSMIT_EXIT(Retval,TxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_TXCONFIRMATION_ENTRY
/** \brief Entry point of function IpduM_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_TXCONFIRMATION_ENTRY(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_TXCONFIRMATION_EXIT
/** \brief Exit point of function IpduM_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_TXCONFIRMATION_EXIT(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_PROCESSREQUESTPDU_ENTRY
/** \brief Entry point of function IpduM_ProcessRequestPdu()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_IPDUM_PROCESSREQUESTPDU_ENTRY(PdumRxPduId,RxRequestPduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_IPDUM_PROCESSREQUESTPDU_EXIT
/** \brief Exit point of function IpduM_ProcessRequestPdu()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_IPDUM_PROCESSREQUESTPDU_EXIT(Retval,PdumRxPduId,RxRequestPduInfoPtr)
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

#endif /* ifndef IPDUM_TRACE_H */
/*==================[end of file]===========================================*/

