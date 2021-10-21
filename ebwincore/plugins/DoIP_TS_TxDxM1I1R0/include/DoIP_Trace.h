/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DOIP_TRACE_H
#define DOIP_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h> /* AUTOSAR standard types */


#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
/* give tests a chance to disable the internal trace macros completely */
#ifndef DOIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING
#include <DoIp_Trace_Internal.h>

/*==================[macros]================================================*/
/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function DoIP_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ISVALIDCONFIG_EXIT
/** \brief Exit point of function DoIP_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_INIT_ENTRY
/** \brief Entry point of function DoIP_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_INIT_ENTRY(DoIP_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_INIT_EXIT
/** \brief Exit point of function DoIP_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_INIT_EXIT(DoIP_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAINFUNCTION_ENTRY
/** \brief Entry point of function DoIP_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAINFUNCTION_EXIT
/** \brief Exit point of function DoIP_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function DoIP_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_MAINFUNCTIONTX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function DoIP_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_MAINFUNCTIONTX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETVERSIONINFO_ENTRY
/** \brief Entry point of function DoIP_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETVERSIONINFO_ENTRY(VersionInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETVERSIONINFO_EXIT
/** \brief Exit point of function DoIP_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETVERSIONINFO_EXIT(VersionInfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_TPTRANSMIT_ENTRY
/** \brief Entry point of function DoIP_TpTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_TPTRANSMIT_ENTRY(DoIPPduRTxId,DoIPPduRTxInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_TPTRANSMIT_EXIT
/** \brief Exit point of function DoIP_TpTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_TPTRANSMIT_EXIT(Retval,DoIPPduRTxId,DoIPPduRTxInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CANCELTRANSMIT_ENTRY
/** \brief Entry point of function DoIP_CancelTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CANCELTRANSMIT_ENTRY(DoIPPduRTxId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CANCELTRANSMIT_EXIT
/** \brief Exit point of function DoIP_CancelTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_CANCELTRANSMIT_EXIT(Retval,DoIPPduRTxId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CANCELRECEIVE_ENTRY
/** \brief Entry point of function DoIP_CancelReceive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_CANCELRECEIVE_ENTRY(DoIPPduRRxId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_CANCELRECEIVE_EXIT
/** \brief Exit point of function DoIP_CancelReceive()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_CANCELRECEIVE_EXIT(Retval,DoIPPduRRxId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPCOPYTXDATA_ENTRY
/** \brief Entry point of function DoIP_SoAdTpCopyTxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPCOPYTXDATA_ENTRY(TxPduId,PduInfoPtr,Retry,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPCOPYTXDATA_EXIT
/** \brief Exit point of function DoIP_SoAdTpCopyTxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SOADTPCOPYTXDATA_EXIT(Retval,TxPduId,PduInfoPtr,Retry,AvailableDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPTXCONFIRMATION_ENTRY
/** \brief Entry point of function DoIP_SoAdTpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPTXCONFIRMATION_ENTRY(TxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPTXCONFIRMATION_EXIT
/** \brief Exit point of function DoIP_SoAdTpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPTXCONFIRMATION_EXIT(TxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPCOPYRXDATA_ENTRY
/** \brief Entry point of function DoIP_SoAdTpCopyRxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPCOPYRXDATA_ENTRY(RxPduId,PduInfoPtr,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPCOPYRXDATA_EXIT
/** \brief Exit point of function DoIP_SoAdTpCopyRxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SOADTPCOPYRXDATA_EXIT(Retval,RxPduId,PduInfoPtr,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPSTARTOFRECEPTION_ENTRY
/** \brief Entry point of function DoIP_SoAdTpStartOfReception()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPSTARTOFRECEPTION_ENTRY(RxPduId,TpSduLength,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPSTARTOFRECEPTION_EXIT
/** \brief Exit point of function DoIP_SoAdTpStartOfReception()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_SOADTPSTARTOFRECEPTION_EXIT(Retval,RxPduId,TpSduLength,BufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPRXINDICATION_ENTRY
/** \brief Entry point of function DoIP_SoAdTpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPRXINDICATION_ENTRY(RxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADTPRXINDICATION_EXIT
/** \brief Exit point of function DoIP_SoAdTpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADTPRXINDICATION_EXIT(RxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOCONMODECHG_ENTRY
/** \brief Entry point of function DoIP_SoConModeChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOCONMODECHG_ENTRY(SoConId,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOCONMODECHG_EXIT
/** \brief Exit point of function DoIP_SoConModeChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOCONMODECHG_EXIT(SoConId,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_LOCALIPADDRASSIGNMENTCHG_ENTRY
/** \brief Entry point of function DoIP_LocalIpAddrAssignmentChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_LOCALIPADDRASSIGNMENTCHG_ENTRY(SoConId,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_LOCALIPADDRASSIGNMENTCHG_EXIT
/** \brief Exit point of function DoIP_LocalIpAddrAssignmentChg()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_LOCALIPADDRASSIGNMENTCHG_EXIT(SoConId,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ACTIVATIONLINESWITCHACTIVE_ENTRY
/** \brief Entry point of function DoIP_ActivationLineSwitchActive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ACTIVATIONLINESWITCHACTIVE_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ACTIVATIONLINESWITCHACTIVE_EXIT
/** \brief Exit point of function DoIP_ActivationLineSwitchActive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ACTIVATIONLINESWITCHACTIVE_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ACTIVATIONLINESWITCHINACTIVE_ENTRY
/** \brief Entry point of function DoIP_ActivationLineSwitchInactive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ACTIVATIONLINESWITCHINACTIVE_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_ACTIVATIONLINESWITCHINACTIVE_EXIT
/** \brief Exit point of function DoIP_ActivationLineSwitchInactive()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_ACTIVATIONLINESWITCHINACTIVE_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADIFTXCONFIRMATION_ENTRY
/** \brief Entry point of function DoIP_SoAdIfTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADIFTXCONFIRMATION_ENTRY(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADIFTXCONFIRMATION_EXIT
/** \brief Exit point of function DoIP_SoAdIfTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADIFTXCONFIRMATION_EXIT(TxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADIFRXINDICATION_ENTRY
/** \brief Entry point of function DoIP_SoAdIfRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADIFRXINDICATION_ENTRY(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_SOADIFRXINDICATION_EXIT
/** \brief Exit point of function DoIP_SoAdIfRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_SOADIFRXINDICATION_EXIT(RxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETANDRESETMEASUREMENTDATA_ENTRY
/** \brief Entry point of function DoIP_GetAndResetMeasurementData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DOIP_GETANDRESETMEASUREMENTDATA_ENTRY(MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DOIP_GETANDRESETMEASUREMENTDATA_EXIT
/** \brief Exit point of function DoIP_GetAndResetMeasurementData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DOIP_GETANDRESETMEASUREMENTDATA_EXIT(Retval,MeasurementIdx,MeasurementResetNeeded,MeasurementDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/*==================[type definitions]======================================*/

/*==================[external function declarations]========================*/

/*==================[external constants]====================================*/

/*==================[external data]=========================================*/

/*==================[internal function definitions]=========================*/
#else /* DOIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#define DOIP_TRACE_H_DISABLED
#endif /* DOIP_DISABLE_INTERNAL_TRACE_MACROS_FOR_TESTING */
#endif /* ifndef DOIP_TRACE_H */
/*==================[end of file]===========================================*/
