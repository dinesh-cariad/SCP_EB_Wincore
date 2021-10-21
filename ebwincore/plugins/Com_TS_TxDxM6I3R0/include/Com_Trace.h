/**
 * \file
 *
 * \brief AUTOSAR Com
 *
 * This file contains the implementation of the AUTOSAR
 * module Com.
 *
 * \version 6.3.50
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COM_TRACE_H
#define COM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <Com_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_INIT_ENTRY
/** \brief Entry point of function Com_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_INIT_ENTRY(Com_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_INIT_EXIT
/** \brief Exit point of function Com_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_INIT_EXIT(Com_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function Com_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_ISVALIDCONFIG_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_ISVALIDCONFIG_EXIT
/** \brief Exit point of function Com_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_ISVALIDCONFIG_EXIT(Retval,ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Com_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_GETVERSIONINFO_ENTRY(versionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETVERSIONINFO_EXIT
/** \brief Exit point of function Com_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_GETVERSIONINFO_EXIT(versionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_DEINIT_ENTRY
/** \brief Entry point of function Com_DeInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_DEINIT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_DEINIT_EXIT
/** \brief Exit point of function Com_DeInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_DEINIT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_IPDUGROUPCONTROL_ENTRY
/** \brief Entry point of function Com_IpduGroupControl()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_IPDUGROUPCONTROL_ENTRY(ipduGroupVector,Initialize)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_IPDUGROUPCONTROL_EXIT
/** \brief Exit point of function Com_IpduGroupControl()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_IPDUGROUPCONTROL_EXIT(ipduGroupVector,Initialize)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEPTIONDMCONTROL_ENTRY
/** \brief Entry point of function Com_ReceptionDMControl()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEPTIONDMCONTROL_ENTRY(ipduGroupVector)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEPTIONDMCONTROL_EXIT
/** \brief Exit point of function Com_ReceptionDMControl()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEPTIONDMCONTROL_EXIT(ipduGroupVector)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_CLEARIPDUGROUPVECTOR_ENTRY
/** \brief Entry point of function Com_ClearIpduGroupVector()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_CLEARIPDUGROUPVECTOR_ENTRY(ipduGroupVector)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_CLEARIPDUGROUPVECTOR_EXIT
/** \brief Exit point of function Com_ClearIpduGroupVector()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_CLEARIPDUGROUPVECTOR_EXIT(ipduGroupVector)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SETIPDUGROUP_ENTRY
/** \brief Entry point of function Com_SetIpduGroup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SETIPDUGROUP_ENTRY(ipduGroupVector,ipduGroupId,bitval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SETIPDUGROUP_EXIT
/** \brief Exit point of function Com_SetIpduGroup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SETIPDUGROUP_EXIT(ipduGroupVector,ipduGroupId,bitval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETSTATUS_ENTRY
/** \brief Entry point of function Com_GetStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_GETSTATUS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETSTATUS_EXIT
/** \brief Exit point of function Com_GetStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_GETSTATUS_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETCONFIGURATIONID_ENTRY
/** \brief Entry point of function Com_GetConfigurationId()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_GETCONFIGURATIONID_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETCONFIGURATIONID_EXIT
/** \brief Exit point of function Com_GetConfigurationId()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_GETCONFIGURATIONID_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDSIGNAL_ENTRY
/** \brief Entry point of function Com_SendSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SENDSIGNAL_ENTRY(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDSIGNAL_EXIT
/** \brief Exit point of function Com_SendSignal()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_SENDSIGNAL_EXIT(Retval,SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDDYNSIGNAL_ENTRY
/** \brief Entry point of function Com_SendDynSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SENDDYNSIGNAL_ENTRY(SignalId,SignalDataPtr,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDDYNSIGNAL_EXIT
/** \brief Exit point of function Com_SendDynSignal()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_SENDDYNSIGNAL_EXIT(Retval,SignalId,SignalDataPtr,Length)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNAL_ENTRY
/** \brief Entry point of function Com_ReceiveSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVESIGNAL_ENTRY(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNAL_EXIT
/** \brief Exit point of function Com_ReceiveSignal()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_RECEIVESIGNAL_EXIT(Retval,SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNALGENERIC_ENTRY
/** \brief Entry point of function Com_ReceiveSignalGeneric()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVESIGNALGENERIC_ENTRY(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNALGENERIC_EXIT
/** \brief Exit point of function Com_ReceiveSignalGeneric()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_RECEIVESIGNALGENERIC_EXIT(Retval,SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVEDYNSIGNAL_ENTRY
/** \brief Entry point of function Com_ReceiveDynSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVEDYNSIGNAL_ENTRY(SignalId,SignalDataPtr,LengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVEDYNSIGNAL_EXIT
/** \brief Exit point of function Com_ReceiveDynSignal()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_RECEIVEDYNSIGNAL_EXIT(Retval,SignalId,SignalDataPtr,LengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_MAINFUNCTIONTX_ENTRY
/** \brief Entry point of function Com_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_MAINFUNCTIONTX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_MAINFUNCTIONTX_EXIT
/** \brief Exit point of function Com_MainFunctionTx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_MAINFUNCTIONTX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_MAINFUNCTIONROUTESIGNALS_ENTRY
/** \brief Entry point of function Com_MainFunctionRouteSignals()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_MAINFUNCTIONROUTESIGNALS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_MAINFUNCTIONROUTESIGNALS_EXIT
/** \brief Exit point of function Com_MainFunctionRouteSignals()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_MAINFUNCTIONROUTESIGNALS_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_UPDATESHADOWSIGNAL_ENTRY
/** \brief Entry point of function Com_UpdateShadowSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_UPDATESHADOWSIGNAL_ENTRY(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_UPDATESHADOWSIGNAL_EXIT
/** \brief Exit point of function Com_UpdateShadowSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_UPDATESHADOWSIGNAL_EXIT(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDSIGNALGROUP_ENTRY
/** \brief Entry point of function Com_SendSignalGroup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SENDSIGNALGROUP_ENTRY(SignalGroupId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDSIGNALGROUP_EXIT
/** \brief Exit point of function Com_SendSignalGroup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_SENDSIGNALGROUP_EXIT(Retval,SignalGroupId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNALGROUP_ENTRY
/** \brief Entry point of function Com_ReceiveSignalGroup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVESIGNALGROUP_ENTRY(SignalGroupId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNALGROUP_EXIT
/** \brief Exit point of function Com_ReceiveSignalGroup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_RECEIVESIGNALGROUP_EXIT(Retval,SignalGroupId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESHADOWSIGNAL_ENTRY
/** \brief Entry point of function Com_ReceiveShadowSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVESHADOWSIGNAL_ENTRY(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESHADOWSIGNAL_EXIT
/** \brief Exit point of function Com_ReceiveShadowSignal()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVESHADOWSIGNAL_EXIT(SignalId,SignalDataPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDSIGNALGROUPARRAY_ENTRY
/** \brief Entry point of function Com_SendSignalGroupArray()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SENDSIGNALGROUPARRAY_ENTRY(SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SENDSIGNALGROUPARRAY_EXIT
/** \brief Exit point of function Com_SendSignalGroupArray()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_SENDSIGNALGROUPARRAY_EXIT(Retval,SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLength)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNALGROUPARRAY_ENTRY
/** \brief Entry point of function Com_ReceiveSignalGroupArray()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RECEIVESIGNALGROUPARRAY_ENTRY(SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RECEIVESIGNALGROUPARRAY_EXIT
/** \brief Exit point of function Com_ReceiveSignalGroupArray()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_RECEIVESIGNALGROUPARRAY_EXIT(Retval,SignalGroupId,SignalGroupArrayPtr,SignalGroupArrayLengthPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TRIGGERIPDUSEND_ENTRY
/** \brief Entry point of function Com_TriggerIPDUSend()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TRIGGERIPDUSEND_ENTRY(ComTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TRIGGERIPDUSEND_EXIT
/** \brief Exit point of function Com_TriggerIPDUSend()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TRIGGERIPDUSEND_EXIT(ComTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SWITCHIPDUTXMODE_ENTRY
/** \brief Entry point of function Com_SwitchIpduTxMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SWITCHIPDUTXMODE_ENTRY(PduId,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_SWITCHIPDUTXMODE_EXIT
/** \brief Exit point of function Com_SwitchIpduTxMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_SWITCHIPDUTXMODE_EXIT(PduId,Mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_MAINFUNCTIONRX_ENTRY
/** \brief Entry point of function Com_MainFunctionRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_MAINFUNCTIONRX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_MAINFUNCTIONRX_EXIT
/** \brief Exit point of function Com_MainFunctionRx()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_MAINFUNCTIONRX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TRIGGERTRANSMIT_ENTRY
/** \brief Entry point of function Com_TriggerTransmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TRIGGERTRANSMIT_ENTRY(ComTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TRIGGERTRANSMIT_EXIT
/** \brief Exit point of function Com_TriggerTransmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COM_TRIGGERTRANSMIT_EXIT(Retval,ComTxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RXINDICATION_ENTRY
/** \brief Entry point of function Com_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RXINDICATION_ENTRY(ComRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_RXINDICATION_EXIT
/** \brief Exit point of function Com_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_RXINDICATION_EXIT(ComRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETRXIPDUBUFFER_ENTRY
/** \brief Entry point of function Com_GetRxIPduBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_GETRXIPDUBUFFER_ENTRY(ComRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_GETRXIPDUBUFFER_EXIT
/** \brief Exit point of function Com_GetRxIPduBuffer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_GETRXIPDUBUFFER_EXIT(RetVal,ComRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TXCONFIRMATION_ENTRY
/** \brief Entry point of function Com_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TXCONFIRMATION_ENTRY(ComTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TXCONFIRMATION_EXIT
/** \brief Exit point of function Com_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TXCONFIRMATION_EXIT(ComTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TPTXCONFIRMATION_ENTRY
/** \brief Entry point of function Com_TpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TPTXCONFIRMATION_ENTRY(PduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TPTXCONFIRMATION_EXIT
/** \brief Exit point of function Com_TpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TPTXCONFIRMATION_EXIT(PduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_COPYTXDATA_ENTRY
/** \brief Entry point of function Com_CopyTxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_COPYTXDATA_ENTRY(PduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_COPYTXDATA_EXIT
/** \brief Exit point of function Com_CopyTxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_COPYTXDATA_EXIT(RetVal,PduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_STARTOFRECEPTION_ENTRY
/** \brief Entry point of function Com_StartOfReception()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_STARTOFRECEPTION_ENTRY(ComRxPduId,TpSduLength,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_STARTOFRECEPTION_EXIT
/** \brief Exit point of function Com_StartOfReception()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_STARTOFRECEPTION_EXIT(RetVal,ComRxPduId,TpSduLength,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_COPYRXDATA_ENTRY
/** \brief Entry point of function Com_CopyRxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_COPYRXDATA_ENTRY(PduId,PduInfoPointer,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_COPYRXDATA_EXIT
/** \brief Exit point of function Com_CopyRxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_COPYRXDATA_EXIT(RetVal,PduId,PduInfoPointer,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TPRXINDICATION_ENTRY
/** \brief Entry point of function Com_TpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TPRXINDICATION_ENTRY(PduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COM_TPRXINDICATION_EXIT
/** \brief Exit point of function Com_TpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COM_TPRXINDICATION_EXIT(PduId,Result)
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

#endif /* ifndef COM_TRACE_H */
/*==================[end of file]===========================================*/
