/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef LDCOM_TRACE_H
#define LDCOM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <LdCom_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function LdCom_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_GETVERSIONINFO_EXIT
/** \brief Exit point of function LdCom_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_INIT_ENTRY
/** \brief Entry point of function LdCom_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_INIT_ENTRY(config)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_INIT_EXIT
/** \brief Exit point of function LdCom_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_INIT_EXIT(config)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_DEINIT_ENTRY
/** \brief Entry point of function LdCom_DeInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_DEINIT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_DEINIT_EXIT
/** \brief Exit point of function LdCom_DeInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_DEINIT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TPTRANSMIT_ENTRY
/** \brief Entry point of function LdCom_TpTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TPTRANSMIT_ENTRY(Id,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TPTRANSMIT_EXIT
/** \brief Exit point of function LdCom_TpTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_TPTRANSMIT_EXIT(Retval,Id,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_IFTRANSMIT_ENTRY
/** \brief Entry point of function LdCom_IfTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_IFTRANSMIT_ENTRY(Id,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_IFTRANSMIT_EXIT
/** \brief Exit point of function LdCom_IfTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_IFTRANSMIT_EXIT(Retval,Id,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function LdCom_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_ISVALIDCONFIG_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_ISVALIDCONFIG_EXIT
/** \brief Exit point of function LdCom_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_ISVALIDCONFIG_EXIT(Retval,ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_STARTOFRECEPTION_ENTRY
/** \brief Entry point of function LdCom_StartOfReception()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_STARTOFRECEPTION_ENTRY(Id,SduLength,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_STARTOFRECEPTION_EXIT
/** \brief Exit point of function LdCom_StartOfReception()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_STARTOFRECEPTION_EXIT(Retval,Id,SduLength,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_COPYRXDATA_ENTRY
/** \brief Entry point of function LdCom_CopyRxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_COPYRXDATA_ENTRY(Id,Info,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_COPYRXDATA_EXIT
/** \brief Exit point of function LdCom_CopyRxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_COPYRXDATA_EXIT(Retval,Id,Info,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TPRXINDICATION_ENTRY
/** \brief Entry point of function LdCom_TpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TPRXINDICATION_ENTRY(Id,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TPRXINDICATION_EXIT
/** \brief Exit point of function LdCom_TpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TPRXINDICATION_EXIT(Id,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_COPYTXDATA_ENTRY
/** \brief Entry point of function LdCom_CopyTxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_COPYTXDATA_ENTRY(Id,Info,Retry,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_COPYTXDATA_EXIT
/** \brief Exit point of function LdCom_CopyTxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_COPYTXDATA_EXIT(Retval,Id,Info,Retry,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TPTXCONFIRMATION_ENTRY
/** \brief Entry point of function LdCom_TpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TPTXCONFIRMATION_ENTRY(Id,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TPTXCONFIRMATION_EXIT
/** \brief Exit point of function LdCom_TpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TPTXCONFIRMATION_EXIT(Id,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_RXINDICATION_ENTRY
/** \brief Entry point of function LdCom_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_RXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_RXINDICATION_EXIT
/** \brief Exit point of function LdCom_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_RXINDICATION_EXIT(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function LdCom_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TRIGGERTRANSMIT_ENTRY(Id,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function LdCom_TriggerTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_LDCOM_TRIGGERTRANSMIT_EXIT(Retval,Id,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TXCONFIRMATION_ENTRY
/** \brief Entry point of function LdCom_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TXCONFIRMATION_ENTRY(Id)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_LDCOM_TXCONFIRMATION_EXIT
/** \brief Exit point of function LdCom_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_LDCOM_TXCONFIRMATION_EXIT(Id)
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

#endif /* ifndef LDCOM_TRACE_H */
/*==================[end of file]===========================================*/
