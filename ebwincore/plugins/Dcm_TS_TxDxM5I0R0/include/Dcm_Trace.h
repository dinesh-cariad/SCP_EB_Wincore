/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef DCM_TRACE_H
#define DCM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <Dcm_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_INIT_ENTRY
/** \brief Entry point of function Dcm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_INIT_EXIT
/** \brief Exit point of function Dcm_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Dcm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_GETVERSIONINFO_ENTRY(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETVERSIONINFO_EXIT
/** \brief Exit point of function Dcm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_GETVERSIONINFO_EXIT(versioninfo)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETACTIVEPROTOCOL_ENTRY
/** \brief Entry point of function Dcm_GetActiveProtocol()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_GETACTIVEPROTOCOL_ENTRY(ActiveProtocol)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETACTIVEPROTOCOL_EXIT
/** \brief Exit point of function Dcm_GetActiveProtocol()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_GETACTIVEPROTOCOL_EXIT(Retval,ActiveProtocol)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETSESCTRLTYPE_ENTRY
/** \brief Entry point of function Dcm_GetSesCtrlType()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_GETSESCTRLTYPE_ENTRY(SesCtrlType)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETSESCTRLTYPE_EXIT
/** \brief Exit point of function Dcm_GetSesCtrlType()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_GETSESCTRLTYPE_EXIT(Retval,SesCtrlType)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETSECURITYLEVEL_ENTRY
/** \brief Entry point of function Dcm_GetSecurityLevel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_GETSECURITYLEVEL_ENTRY(SecLevel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETSECURITYLEVEL_EXIT
/** \brief Exit point of function Dcm_GetSecurityLevel()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_GETSECURITYLEVEL_EXIT(Retval,SecLevel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_RESETTODEFAULTSESSION_ENTRY
/** \brief Entry point of function Dcm_ResetToDefaultSession()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_RESETTODEFAULTSESSION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_RESETTODEFAULTSESSION_EXIT
/** \brief Exit point of function Dcm_ResetToDefaultSession()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_RESETTODEFAULTSESSION_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SETACTIVEDIAGNOSTIC_ENTRY
/** \brief Entry point of function Dcm_SetActiveDiagnostic()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_SETACTIVEDIAGNOSTIC_ENTRY(active)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SETACTIVEDIAGNOSTIC_EXIT
/** \brief Exit point of function Dcm_SetActiveDiagnostic()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_SETACTIVEDIAGNOSTIC_EXIT(Retval,active)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TRIGGERONEVENT_ENTRY
/** \brief Entry point of function Dcm_TriggerOnEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TRIGGERONEVENT_ENTRY(RoeEventId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TRIGGERONEVENT_EXIT
/** \brief Exit point of function Dcm_TriggerOnEvent()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_TRIGGERONEVENT_EXIT(Retval,RoeEventId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_MAINFUNCTION_ENTRY
/** \brief Entry point of function Dcm_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_MAINFUNCTION_EXIT
/** \brief Exit point of function Dcm_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DEMTRIGGERONDTCSTATUS_ENTRY
/** \brief Entry point of function Dcm_DemTriggerOnDTCStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DEMTRIGGERONDTCSTATUS_ENTRY(DTC,DTCStatusOld,DTCStatusNew)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DEMTRIGGERONDTCSTATUS_EXIT
/** \brief Exit point of function Dcm_DemTriggerOnDTCStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DEMTRIGGERONDTCSTATUS_EXIT(Retval,DTC,DTCStatusOld,DTCStatusNew)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_EXTERNALPROCESSINGDONE_ENTRY
/** \brief Entry point of function Dcm_ExternalProcessingDone()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_EXTERNALPROCESSINGDONE_ENTRY(pMsgContext)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_EXTERNALPROCESSINGDONE_EXIT
/** \brief Exit point of function Dcm_ExternalProcessingDone()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_EXTERNALPROCESSINGDONE_EXIT(pMsgContext)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_HARDRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_HardResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_HARDRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_HARDRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_HardResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_HARDRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_KEYONOFFRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_KeyOnOffResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_KEYONOFFRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_KEYONOFFRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_KeyOnOffResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_KEYONOFFRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SOFTRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_SoftResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_SOFTRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SOFTRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_SoftResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_SOFTRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRapidPowerShutdownResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRapidPowerShutdownResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRapidPowerShutdownResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRapidPowerShutdownResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERAPIDPOWERSHUTDOWNRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_BOOTLOADERRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_BootloaderResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_BOOTLOADERRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_BOOTLOADERRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_BootloaderResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_BOOTLOADERRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SSBOOTLOADERRESETMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_SsBootloaderResetModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_SSBOOTLOADERRESETMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SSBOOTLOADERRESETMODEENTRY_EXIT
/** \brief Exit point of function Dcm_SsBootloaderResetModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_SSBOOTLOADERRESETMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXTXNORMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRxTxNormModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERXTXNORMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXTXNORMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRxTxNormModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERXTXNORMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXDISABLETXNORMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRxDisableTxNormModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERXDISABLETXNORMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXDISABLETXNORMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRxDisableTxNormModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERXDISABLETXNORMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXENABLETXNORMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRxEnableTxNormModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERXENABLETXNORMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXENABLETXNORMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRxEnableTxNormModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERXENABLETXNORMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXTXNORMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRxTxNormModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERXTXNORMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXTXNORMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRxTxNormModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERXTXNORMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXTXNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRxTxNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERXTXNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXTXNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRxTxNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERXTXNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXDISABLETXNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRxDisableTxNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERXDISABLETXNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXDISABLETXNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRxDisableTxNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERXDISABLETXNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXENABLETXNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRxEnableTxNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERXENABLETXNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXENABLETXNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRxEnableTxNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERXENABLETXNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXTXNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRxTxNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERXTXNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXTXNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRxTxNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERXTXNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXTXNORMNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRxTxNormNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERXTXNORMNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXTXNORMNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRxTxNormNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERXTXNORMNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXDISABLETXNORMNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_EnableRxDisableTxNormNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_ENABLERXDISABLETXNORMNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_ENABLERXDISABLETXNORMNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_EnableRxDisableTxNormNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_ENABLERXDISABLETXNORMNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXENABLETXNORMNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRxEnableTxNormNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERXENABLETXNORMNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXENABLETXNORMNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRxEnableTxNormNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERXENABLETXNORMNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXTXNORMNMMODEENTRY_ENTRY
/** \brief Entry point of function Dcm_DisableRxTxNormNmModeEntry()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_DISABLERXTXNORMNMMODEENTRY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_DISABLERXTXNORMNMMODEENTRY_EXIT
/** \brief Exit point of function Dcm_DisableRxTxNormNmModeEntry()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_DISABLERXTXNORMNMMODEENTRY_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_PROCESSREQUESTDOWNLOAD_ENTRY
/** \brief Entry point of function Dcm_ProcessRequestDownload()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_PROCESSREQUESTDOWNLOAD_ENTRY(OpStatus,DataFormatIdentifier,MemoryAddress,MemorySize,BlockLength,ErrorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_PROCESSREQUESTDOWNLOAD_EXIT
/** \brief Exit point of function Dcm_ProcessRequestDownload()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_PROCESSREQUESTDOWNLOAD_EXIT(Retval,OpStatus,DataFormatIdentifier,MemoryAddress,MemorySize,BlockLength,ErrorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_PROCESSREQUESTUPLOAD_ENTRY
/** \brief Entry point of function Dcm_ProcessRequestUpload()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_PROCESSREQUESTUPLOAD_ENTRY(OpStatus,DataFormatIdentifier,MemoryAddress,MemorySize,ErrorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_PROCESSREQUESTUPLOAD_EXIT
/** \brief Exit point of function Dcm_ProcessRequestUpload()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_PROCESSREQUESTUPLOAD_EXIT(Retval,OpStatus,DataFormatIdentifier,MemoryAddress,MemorySize,ErrorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_READMEMORY_ENTRY
/** \brief Entry point of function Dcm_ReadMemory()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_READMEMORY_ENTRY(OpStatus,MemoryIdentifier,MemoryAddress,MemorySize,MemoryData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_READMEMORY_EXIT
/** \brief Exit point of function Dcm_ReadMemory()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_READMEMORY_EXIT(Retval,OpStatus,MemoryIdentifier,MemoryAddress,MemorySize,MemoryData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_WRITEMEMORY_ENTRY
/** \brief Entry point of function Dcm_WriteMemory()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_WRITEMEMORY_ENTRY(OpStatus,MemoryIdentifier,MemoryAddress,MemorySize,MemoryData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_WRITEMEMORY_EXIT
/** \brief Exit point of function Dcm_WriteMemory()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_WRITEMEMORY_EXIT(Retval,OpStatus,MemoryIdentifier,MemoryAddress,MemorySize,MemoryData)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_PROCESSREQUESTTRANSFEREXIT_ENTRY
/** \brief Entry point of function Dcm_ProcessRequestTransferExit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_PROCESSREQUESTTRANSFEREXIT_ENTRY(OpStatus,ParameterRecord,ParameterRecordSize,ErrorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_PROCESSREQUESTTRANSFEREXIT_EXIT
/** \brief Exit point of function Dcm_ProcessRequestTransferExit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_PROCESSREQUESTTRANSFEREXIT_EXIT(Retval,OpStatus,ParameterRecord,ParameterRecordSize,ErrorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SETPROGCONDITIONS_ENTRY
/** \brief Entry point of function Dcm_SetProgConditions()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_SETPROGCONDITIONS_ENTRY(ProgConditions)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_SETPROGCONDITIONS_EXIT
/** \brief Exit point of function Dcm_SetProgConditions()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_SETPROGCONDITIONS_EXIT(Retval,ProgConditions)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETPROGCONDITIONS_ENTRY
/** \brief Entry point of function Dcm_GetProgConditions()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_GETPROGCONDITIONS_ENTRY(ProgConditions)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_GETPROGCONDITIONS_EXIT
/** \brief Exit point of function Dcm_GetProgConditions()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_GETPROGCONDITIONS_EXIT(Retval,ProgConditions)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_EXTERNALSETNEGRESPONSE_ENTRY
/** \brief Entry point of function Dcm_ExternalSetNegResponse()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_EXTERNALSETNEGRESPONSE_ENTRY(pMsgContext,errorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_EXTERNALSETNEGRESPONSE_EXIT
/** \brief Exit point of function Dcm_ExternalSetNegResponse()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_EXTERNALSETNEGRESPONSE_EXIT(pMsgContext,errorCode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_STARTOFRECEPTION_ENTRY
/** \brief Entry point of function Dcm_StartOfReception()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_STARTOFRECEPTION_ENTRY(DcmRxPduId,TpSduLength,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_STARTOFRECEPTION_EXIT
/** \brief Exit point of function Dcm_StartOfReception()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_STARTOFRECEPTION_EXIT(Retval,DcmRxPduId,TpSduLength,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COPYRXDATA_ENTRY
/** \brief Entry point of function Dcm_CopyRxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COPYRXDATA_ENTRY(DcmRxPduId,PduInfoPtr,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COPYRXDATA_EXIT
/** \brief Exit point of function Dcm_CopyRxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_COPYRXDATA_EXIT(Retval,DcmRxPduId,PduInfoPtr,RxBufferSizePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TPRXINDICATION_ENTRY
/** \brief Entry point of function Dcm_TpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TPRXINDICATION_ENTRY(DcmRxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TPRXINDICATION_EXIT
/** \brief Exit point of function Dcm_TpRxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TPRXINDICATION_EXIT(DcmRxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_RXINDICATION_ENTRY
/** \brief Entry point of function Dcm_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_RXINDICATION_ENTRY(DcmRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_RXINDICATION_EXIT
/** \brief Exit point of function Dcm_RxIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_RXINDICATION_EXIT(DcmRxPduId,PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COPYTXDATA_ENTRY
/** \brief Entry point of function Dcm_CopyTxData()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COPYTXDATA_ENTRY(DcmTxPduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COPYTXDATA_EXIT
/** \brief Exit point of function Dcm_CopyTxData()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_DCM_COPYTXDATA_EXIT(Retval,DcmTxPduId,PduInfoPtr,RetryInfoPtr,TxDataCntPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TPTXCONFIRMATION_ENTRY
/** \brief Entry point of function Dcm_TpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TPTXCONFIRMATION_ENTRY(DcmTxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TPTXCONFIRMATION_EXIT
/** \brief Exit point of function Dcm_TpTxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TPTXCONFIRMATION_EXIT(DcmTxPduId,Result)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TXCONFIRMATION_ENTRY
/** \brief Entry point of function Dcm_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TXCONFIRMATION_ENTRY(DcmTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_TXCONFIRMATION_EXIT
/** \brief Exit point of function Dcm_TxConfirmation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_TXCONFIRMATION_EXIT(DcmTxPduId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COMM_NOCOMMODEENTERED_ENTRY
/** \brief Entry point of function Dcm_ComM_NoComModeEntered()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COMM_NOCOMMODEENTERED_ENTRY(NetworkId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COMM_NOCOMMODEENTERED_EXIT
/** \brief Exit point of function Dcm_ComM_NoComModeEntered()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COMM_NOCOMMODEENTERED_EXIT(NetworkId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COMM_SILENTCOMMODEENTERED_ENTRY
/** \brief Entry point of function Dcm_ComM_SilentComModeEntered()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COMM_SILENTCOMMODEENTERED_ENTRY(NetworkId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COMM_SILENTCOMMODEENTERED_EXIT
/** \brief Exit point of function Dcm_ComM_SilentComModeEntered()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COMM_SILENTCOMMODEENTERED_EXIT(NetworkId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COMM_FULLCOMMODEENTERED_ENTRY
/** \brief Entry point of function Dcm_ComM_FullComModeEntered()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COMM_FULLCOMMODEENTERED_ENTRY(NetworkId)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_DCM_COMM_FULLCOMMODEENTERED_EXIT
/** \brief Exit point of function Dcm_ComM_FullComModeEntered()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_DCM_COMM_FULLCOMMODEENTERED_EXIT(NetworkId)
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

#endif /* ifndef DCM_TRACE_H */
/*==================[end of file]===========================================*/
