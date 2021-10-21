/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef FRIF_TRACE_H
#define FRIF_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

#include <FrIf_guards.h> /* Include define guards */
#include <FrIf_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETVERSIONINFO_ENTRY
/** \brief Entry point of function FrIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETVERSIONINFO_ENTRY(FrIf_VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETVERSIONINFO_EXIT
/** \brief Exit point of function FrIf_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETVERSIONINFO_EXIT(FrIf_VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ISVALIDCONFIG_ENTRY
/** \brief Entry point of function FrIf_IsValidConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ISVALIDCONFIG_ENTRY(voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ISVALIDCONFIG_EXIT
/** \brief Exit point of function FrIf_IsValidConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ISVALIDCONFIG_EXIT(Retval,voidConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_INIT_ENTRY
/** \brief Entry point of function FrIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_INIT_ENTRY(FrIf_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_INIT_EXIT
/** \brief Exit point of function FrIf_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_INIT_EXIT(FrIf_ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CONTROLLERINIT_ENTRY
/** \brief Entry point of function FrIf_ControllerInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CONTROLLERINIT_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CONTROLLERINIT_EXIT
/** \brief Exit point of function FrIf_ControllerInit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_CONTROLLERINIT_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_STARTCOMMUNICATION_ENTRY
/** \brief Entry point of function FrIf_StartCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_STARTCOMMUNICATION_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_STARTCOMMUNICATION_EXIT
/** \brief Exit point of function FrIf_StartCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_STARTCOMMUNICATION_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_HALTCOMMUNICATION_ENTRY
/** \brief Entry point of function FrIf_HaltCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_HALTCOMMUNICATION_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_HALTCOMMUNICATION_EXIT
/** \brief Exit point of function FrIf_HaltCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_HALTCOMMUNICATION_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ABORTCOMMUNICATION_ENTRY
/** \brief Entry point of function FrIf_AbortCommunication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ABORTCOMMUNICATION_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ABORTCOMMUNICATION_EXIT
/** \brief Exit point of function FrIf_AbortCommunication()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ABORTCOMMUNICATION_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ALLOWCOLDSTART_ENTRY
/** \brief Entry point of function FrIf_AllowColdstart()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ALLOWCOLDSTART_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ALLOWCOLDSTART_EXIT
/** \brief Exit point of function FrIf_AllowColdstart()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ALLOWCOLDSTART_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETSTATE_ENTRY
/** \brief Entry point of function FrIf_SetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SETSTATE_ENTRY(FrIf_ClstIdx,FrIf_StateTransition)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETSTATE_EXIT
/** \brief Exit point of function FrIf_SetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SETSTATE_EXIT(Retval,FrIf_ClstIdx,FrIf_StateTransition)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETSTATE_ENTRY
/** \brief Entry point of function FrIf_GetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETSTATE_ENTRY(FrIf_ClstIdx,FrIf_StatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETSTATE_EXIT
/** \brief Exit point of function FrIf_GetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETSTATE_EXIT(Retval,FrIf_ClstIdx,FrIf_StatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETWAKEUPCHANNEL_ENTRY
/** \brief Entry point of function FrIf_SetWakeupChannel()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SETWAKEUPCHANNEL_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETWAKEUPCHANNEL_EXIT
/** \brief Exit point of function FrIf_SetWakeupChannel()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SETWAKEUPCHANNEL_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SENDWUP_ENTRY
/** \brief Entry point of function FrIf_SendWUP()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SENDWUP_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SENDWUP_EXIT
/** \brief Exit point of function FrIf_SendWUP()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SENDWUP_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETPOCSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetPOCStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETPOCSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_POCStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETPOCSTATUS_EXIT
/** \brief Exit point of function FrIf_GetPOCStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETPOCSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_POCStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETGLOBALTIME_ENTRY
/** \brief Entry point of function FrIf_GetGlobalTime()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETGLOBALTIME_ENTRY(FrIf_CtrlIdx,FrIf_CyclePtr,FrIf_MacroTickPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETGLOBALTIME_EXIT
/** \brief Exit point of function FrIf_GetGlobalTime()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETGLOBALTIME_EXIT(Retval,FrIf_CtrlIdx,FrIf_CyclePtr,FrIf_MacroTickPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETNMVECTOR_ENTRY
/** \brief Entry point of function FrIf_GetNmVector()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETNMVECTOR_ENTRY(FrIf_CtrlIdx,FrIf_NmVectorPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETNMVECTOR_EXIT
/** \brief Exit point of function FrIf_GetNmVector()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETNMVECTOR_EXIT(Retval,FrIf_CtrlIdx,FrIf_NmVectorPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETMACROTICKDURATION_ENTRY
/** \brief Entry point of function FrIf_GetMacrotickDuration()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETMACROTICKDURATION_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETMACROTICKDURATION_EXIT
/** \brief Exit point of function FrIf_GetMacrotickDuration()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETMACROTICKDURATION_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCYCLELENGTH_ENTRY
/** \brief Entry point of function FrIf_GetCycleLength()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETCYCLELENGTH_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCYCLELENGTH_EXIT
/** \brief Exit point of function FrIf_GetCycleLength()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETCYCLELENGTH_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCHANNELSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetChannelStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETCHANNELSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_ChannelAStatusPtr,FrIf_ChannelBStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCHANNELSTATUS_EXIT
/** \brief Exit point of function FrIf_GetChannelStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETCHANNELSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChannelAStatusPtr,FrIf_ChannelBStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETNUMOFSTARTUPFRAMES_ENTRY
/** \brief Entry point of function FrIf_GetNumOfStartupFrames()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETNUMOFSTARTUPFRAMES_ENTRY(FrIf_CtrlIdx,FrIf_NumOfStartupFramesPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETNUMOFSTARTUPFRAMES_EXIT
/** \brief Exit point of function FrIf_GetNumOfStartupFrames()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETNUMOFSTARTUPFRAMES_EXIT(Retval,FrIf_CtrlIdx,FrIf_NumOfStartupFramesPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETABSOLUTETIMER_ENTRY
/** \brief Entry point of function FrIf_SetAbsoluteTimer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SETABSOLUTETIMER_ENTRY(FrIf_CtrlIdx,FrIf_AbsTimerIdx,FrIf_Cycle,FrIf_Offset)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETABSOLUTETIMER_EXIT
/** \brief Exit point of function FrIf_SetAbsoluteTimer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SETABSOLUTETIMER_EXIT(Retval,FrIf_CtrlIdx,FrIf_AbsTimerIdx,FrIf_Cycle,FrIf_Offset)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CANCELABSOLUTETIMER_ENTRY
/** \brief Entry point of function FrIf_CancelAbsoluteTimer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CANCELABSOLUTETIMER_ENTRY(FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CANCELABSOLUTETIMER_EXIT
/** \brief Exit point of function FrIf_CancelAbsoluteTimer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_CANCELABSOLUTETIMER_EXIT(Retval,FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLEABSOLUTETIMERIRQ_ENTRY
/** \brief Entry point of function FrIf_EnableAbsoluteTimerIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ENABLEABSOLUTETIMERIRQ_ENTRY(FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLEABSOLUTETIMERIRQ_EXIT
/** \brief Exit point of function FrIf_EnableAbsoluteTimerIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ENABLEABSOLUTETIMERIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLEBUSMIRRORING_ENTRY
/** \brief Entry point of function FrIf_EnableBusMirroring()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ENABLEBUSMIRRORING_ENTRY(FrIf_ClstIdx,FrIf_MirroringActive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLEBUSMIRRORING_EXIT
/** \brief Exit point of function FrIf_EnableBusMirroring()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ENABLEBUSMIRRORING_EXIT(Retval,FrIf_ClstIdx,FrIf_MirroringActive)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLETRANSCEIVERBRANCH_ENTRY
/** \brief Entry point of function FrIf_EnableTransceiverBranch()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ENABLETRANSCEIVERBRANCH_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_BranchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLETRANSCEIVERBRANCH_EXIT
/** \brief Exit point of function FrIf_EnableTransceiverBranch()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ENABLETRANSCEIVERBRANCH_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_BranchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETABSOLUTETIMERIRQSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetAbsoluteTimerIRQStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETABSOLUTETIMERIRQSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_AbsTimerIdx,FrIf_IRQStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETABSOLUTETIMERIRQSTATUS_EXIT
/** \brief Exit point of function FrIf_GetAbsoluteTimerIRQStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETABSOLUTETIMERIRQSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_AbsTimerIdx,FrIf_IRQStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ACKABSOLUTETIMERIRQ_ENTRY
/** \brief Entry point of function FrIf_AckAbsoluteTimerIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ACKABSOLUTETIMERIRQ_ENTRY(FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ACKABSOLUTETIMERIRQ_EXIT
/** \brief Exit point of function FrIf_AckAbsoluteTimerIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ACKABSOLUTETIMERIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLEABSOLUTETIMERIRQ_ENTRY
/** \brief Entry point of function FrIf_DisableAbsoluteTimerIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_DISABLEABSOLUTETIMERIRQ_ENTRY(FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLEABSOLUTETIMERIRQ_EXIT
/** \brief Exit point of function FrIf_DisableAbsoluteTimerIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_DISABLEABSOLUTETIMERIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_AbsTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLETRANSCEIVERBRANCH_ENTRY
/** \brief Entry point of function FrIf_DisableTransceiverBranch()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_DISABLETRANSCEIVERBRANCH_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_BranchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLETRANSCEIVERBRANCH_EXIT
/** \brief Exit point of function FrIf_DisableTransceiverBranch()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_DISABLETRANSCEIVERBRANCH_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_BranchIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_TRANSMIT_ENTRY
/** \brief Entry point of function FrIf_Transmit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_TRANSMIT_ENTRY(FrIf_TxPduId,FrIf_PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_TRANSMIT_EXIT
/** \brief Exit point of function FrIf_Transmit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_TRANSMIT_EXIT(Retval,FrIf_TxPduId,FrIf_PduInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETTRANSCEIVERMODE_ENTRY
/** \brief Entry point of function FrIf_SetTransceiverMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SETTRANSCEIVERMODE_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_TrcvMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETTRANSCEIVERMODE_EXIT
/** \brief Exit point of function FrIf_SetTransceiverMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SETTRANSCEIVERMODE_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_TrcvMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETTRANSCEIVERMODE_ENTRY
/** \brief Entry point of function FrIf_GetTransceiverMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETTRANSCEIVERMODE_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_TrcvModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETTRANSCEIVERMODE_EXIT
/** \brief Exit point of function FrIf_GetTransceiverMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETTRANSCEIVERMODE_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_TrcvModePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETTRANSCEIVERWUREASON_ENTRY
/** \brief Entry point of function FrIf_GetTransceiverWUReason()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETTRANSCEIVERWUREASON_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_TrcvWUReasonPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETTRANSCEIVERWUREASON_EXIT
/** \brief Exit point of function FrIf_GetTransceiverWUReason()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETTRANSCEIVERWUREASON_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_TrcvWUReasonPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CLEARTRANSCEIVERWAKEUP_ENTRY
/** \brief Entry point of function FrIf_ClearTransceiverWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CLEARTRANSCEIVERWAKEUP_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CLEARTRANSCEIVERWAKEUP_EXIT
/** \brief Exit point of function FrIf_ClearTransceiverWakeup()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_CLEARTRANSCEIVERWAKEUP_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CHECKWAKEUPBYTRANSCEIVER_ENTRY
/** \brief Entry point of function FrIf_CheckWakeupByTransceiver()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CHECKWAKEUPBYTRANSCEIVER_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CHECKWAKEUPBYTRANSCEIVER_EXIT
/** \brief Exit point of function FrIf_CheckWakeupByTransceiver()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CHECKWAKEUPBYTRANSCEIVER_EXIT(FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISPATCHCOMOPS_ENTRY
/** \brief Entry point of function FrIf_DispatchComOps()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_DISPATCHCOMOPS_ENTRY(FrIf_ClstIdx,FrIf_JobIdx,FrIf_Cycle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISPATCHCOMOPS_EXIT
/** \brief Exit point of function FrIf_DispatchComOps()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_DISPATCHCOMOPS_EXIT(Retval,FrIf_ClstIdx,FrIf_JobIdx,FrIf_Cycle)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_READCCCONFIG_ENTRY
/** \brief Entry point of function FrIf_ReadCCConfig()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_READCCCONFIG_ENTRY(FrIf_CtrlIdx,FrIf_ConfigParamIdx,FrIf_ConfigParamValuePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_READCCCONFIG_EXIT
/** \brief Exit point of function FrIf_ReadCCConfig()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_READCCCONFIG_EXIT(Retval,FrIf_CtrlIdx,FrIf_ConfigParamIdx,FrIf_ConfigParamValuePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ALLSLOTS_ENTRY
/** \brief Entry point of function FrIf_AllSlots()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ALLSLOTS_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ALLSLOTS_EXIT
/** \brief Exit point of function FrIf_AllSlots()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ALLSLOTS_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_RECONFIGLPDU_ENTRY
/** \brief Entry point of function FrIf_ReconfigLPdu()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_RECONFIGLPDU_ENTRY(FrIf_CtrlIdx,FrIf_LPduIdx,FrIf_FrameId,FrIf_ChnlIdx,FrIf_CycleRepetition,FrIf_CycleOffset,FrIf_PayloadLength,FrIf_HeaderCRC)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_RECONFIGLPDU_EXIT
/** \brief Exit point of function FrIf_ReconfigLPdu()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_RECONFIGLPDU_EXIT(Retval,FrIf_CtrlIdx,FrIf_LPduIdx,FrIf_FrameId,FrIf_ChnlIdx,FrIf_CycleRepetition,FrIf_CycleOffset,FrIf_PayloadLength,FrIf_HeaderCRC)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLELPDU_ENTRY
/** \brief Entry point of function FrIf_DisableLPdu()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_DISABLELPDU_ENTRY(FrIf_CtrlIdx,FrIf_LPduIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLELPDU_EXIT
/** \brief Exit point of function FrIf_DisableLPdu()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_DISABLELPDU_EXIT(Retval,FrIf_CtrlIdx,FrIf_LPduIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETWAKEUPRXSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetWakeupRxStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETWAKEUPRXSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_WakeupRxStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETWAKEUPRXSTATUS_EXIT
/** \brief Exit point of function FrIf_GetWakeupRxStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETWAKEUPRXSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_WakeupRxStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCLOCKCORRECTION_ENTRY
/** \brief Entry point of function FrIf_GetClockCorrection()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETCLOCKCORRECTION_ENTRY(FrIf_CtrlIdx,FrIf_RateCorrectionPtr,FrIf_OffsetCorrectionPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCLOCKCORRECTION_EXIT
/** \brief Exit point of function FrIf_GetClockCorrection()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETCLOCKCORRECTION_EXIT(Retval,FrIf_CtrlIdx,FrIf_RateCorrectionPtr,FrIf_OffsetCorrectionPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETSYNCFRAMELIST_ENTRY
/** \brief Entry point of function FrIf_GetSyncFrameList()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETSYNCFRAMELIST_ENTRY(FrIf_CtrlIdx,FrIf_ListSize,FrIf_ChannelAEvenListPtr,FrIf_ChannelBEvenListPtr,FrIf_ChannelAOddListPtr,FrIf_ChannelBOddListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETSYNCFRAMELIST_EXIT
/** \brief Exit point of function FrIf_GetSyncFrameList()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETSYNCFRAMELIST_EXIT(Retval,FrIf_CtrlIdx,FrIf_ListSize,FrIf_ChannelAEvenListPtr,FrIf_ChannelBEvenListPtr,FrIf_ChannelAOddListPtr,FrIf_ChannelBOddListPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETTRANSCEIVERERROR_ENTRY
/** \brief Entry point of function FrIf_GetTransceiverError()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETTRANSCEIVERERROR_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_BranchIdx,FrIf_BusErrorState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETTRANSCEIVERERROR_EXIT
/** \brief Exit point of function FrIf_GetTransceiverError()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETTRANSCEIVERERROR_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_BranchIdx,FrIf_BusErrorState)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETMACROTICKSPERCYCLE_ENTRY
/** \brief Entry point of function FrIf_GetMacroticksPerCycle()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETMACROTICKSPERCYCLE_ENTRY(FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETMACROTICKSPERCYCLE_EXIT
/** \brief Exit point of function FrIf_GetMacroticksPerCycle()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETMACROTICKSPERCYCLE_EXIT(Retval,FrIf_CtrlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_MAINFUNCTION_ENTRY
/** \brief Entry point of function FrIf_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_MAINFUNCTION_ENTRY(FrIf_ClstIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_MAINFUNCTION_EXIT
/** \brief Exit point of function FrIf_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_MAINFUNCTION_EXIT(FrIf_ClstIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_JOBLISTEXEC_ENTRY
/** \brief Entry point of function FrIf_JobListExec()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_JOBLISTEXEC_ENTRY(FrIf_ClstIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_JOBLISTEXEC_EXIT
/** \brief Exit point of function FrIf_JobListExec()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_JOBLISTEXEC_EXIT(FrIf_ClstIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SENDMTS_ENTRY
/** \brief Entry point of function FrIf_SendMTS()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SENDMTS_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SENDMTS_EXIT
/** \brief Exit point of function FrIf_SendMTS()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SENDMTS_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_STOPMTS_ENTRY
/** \brief Entry point of function FrIf_StopMTS()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_STOPMTS_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_STOPMTS_EXIT
/** \brief Exit point of function FrIf_StopMTS()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_STOPMTS_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CHECKMTS_ENTRY
/** \brief Entry point of function FrIf_CheckMTS()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CHECKMTS_ENTRY(FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_MTSStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CHECKMTS_EXIT
/** \brief Exit point of function FrIf_CheckMTS()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_CHECKMTS_EXIT(Retval,FrIf_CtrlIdx,FrIf_ChnlIdx,FrIf_MTSStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCONTROLLERERRORSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetControllerErrorStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETCONTROLLERERRORSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_ControllerErrorStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETCONTROLLERERRORSTATUS_EXIT
/** \brief Exit point of function FrIf_GetControllerErrorStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETCONTROLLERERRORSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_ControllerErrorStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ACKIRQ_ENTRY
/** \brief Entry point of function FrIf_AckIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ACKIRQ_ENTRY(FrIf_CtrlIdx,FrIf_IRQIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ACKIRQ_EXIT
/** \brief Exit point of function FrIf_AckIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ACKIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_IRQIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLEIRQ_ENTRY
/** \brief Entry point of function FrIf_EnableIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ENABLEIRQ_ENTRY(FrIf_CtrlIdx,FrIf_IRQIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLEIRQ_EXIT
/** \brief Exit point of function FrIf_EnableIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ENABLEIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_IRQIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLEIRQ_ENTRY
/** \brief Entry point of function FrIf_DisableIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_DISABLEIRQ_ENTRY(FrIf_CtrlIdx,FrIf_IRQIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLEIRQ_EXIT
/** \brief Exit point of function FrIf_DisableIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_DISABLEIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_IRQIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETIRQSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetIRQStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETIRQSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_IRQIdx,FrIf_IRQStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETIRQSTATUS_EXIT
/** \brief Exit point of function FrIf_GetIRQStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETIRQSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_IRQIdx,FrIf_IRQStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETSYNCSTATE_ENTRY
/** \brief Entry point of function FrIf_GetSyncState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETSYNCSTATE_ENTRY(FrIf_CtrlIdx,FrIf_SyncStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETSYNCSTATE_EXIT
/** \brief Exit point of function FrIf_GetSyncState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETSYNCSTATE_EXIT(Retval,FrIf_CtrlIdx,FrIf_SyncStatePtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETEXTSYNC_ENTRY
/** \brief Entry point of function FrIf_SetExtSync()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SETEXTSYNC_ENTRY(FrIf_CtrlIdx,FrIf_Offset,FrIf_Rate)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETEXTSYNC_EXIT
/** \brief Exit point of function FrIf_SetExtSync()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SETEXTSYNC_EXIT(Retval,FrIf_CtrlIdx,FrIf_Offset,FrIf_Rate)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETRELATIVETIMER_ENTRY
/** \brief Entry point of function FrIf_SetRelativeTimer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_SETRELATIVETIMER_ENTRY(FrIf_CtrlIdx,FrIf_RelTimerIdx,FrIf_Offset)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_SETRELATIVETIMER_EXIT
/** \brief Exit point of function FrIf_SetRelativeTimer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_SETRELATIVETIMER_EXIT(Retval,FrIf_CtrlIdx,FrIf_RelTimerIdx,FrIf_Offset)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CANCELRELATIVETIMER_ENTRY
/** \brief Entry point of function FrIf_CancelRelativeTimer()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_CANCELRELATIVETIMER_ENTRY(FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_CANCELRELATIVETIMER_EXIT
/** \brief Exit point of function FrIf_CancelRelativeTimer()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_CANCELRELATIVETIMER_EXIT(Retval,FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLERELATIVETIMERIRQ_ENTRY
/** \brief Entry point of function FrIf_EnableRelativeTimerIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ENABLERELATIVETIMERIRQ_ENTRY(FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ENABLERELATIVETIMERIRQ_EXIT
/** \brief Exit point of function FrIf_EnableRelativeTimerIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ENABLERELATIVETIMERIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETRELATIVETIMERIRQSTATUS_ENTRY
/** \brief Entry point of function FrIf_GetRelativeTimerIRQStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_GETRELATIVETIMERIRQSTATUS_ENTRY(FrIf_CtrlIdx,FrIf_RelTimerIdx,FrIf_IRQStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_GETRELATIVETIMERIRQSTATUS_EXIT
/** \brief Exit point of function FrIf_GetRelativeTimerIRQStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_GETRELATIVETIMERIRQSTATUS_EXIT(Retval,FrIf_CtrlIdx,FrIf_RelTimerIdx,FrIf_IRQStatusPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ACKRELATIVETIMERIRQ_ENTRY
/** \brief Entry point of function FrIf_AckRelativeTimerIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_ACKRELATIVETIMERIRQ_ENTRY(FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_ACKRELATIVETIMERIRQ_EXIT
/** \brief Exit point of function FrIf_AckRelativeTimerIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_ACKRELATIVETIMERIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLERELATIVETIMERIRQ_ENTRY
/** \brief Entry point of function FrIf_DisableRelativeTimerIRQ()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_FRIF_DISABLERELATIVETIMERIRQ_ENTRY(FrIf_CtrlIdx,FrIf_RelTimerIdx)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_FRIF_DISABLERELATIVETIMERIRQ_EXIT
/** \brief Exit point of function FrIf_DisableRelativeTimerIRQ()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_FRIF_DISABLERELATIVETIMERIRQ_EXIT(Retval,FrIf_CtrlIdx,FrIf_RelTimerIdx)
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

#endif /* ifndef FRIF_TRACE_H */
/*==================[end of file]===========================================*/
