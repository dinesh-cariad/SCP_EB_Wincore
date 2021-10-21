/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef ECUM_TRACE_H
#define ECUM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <EcuM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_SELECTBOOTTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR32_SelectBootTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_SELECTBOOTTARGET_ENTRY(target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_SELECTBOOTTARGET_EXIT
/** \brief Exit point of function EcuM_ASR32_SelectBootTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_SELECTBOOTTARGET_EXIT(Retval,target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_GETBOOTTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR32_GetBootTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_GETBOOTTARGET_ENTRY(target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_GETBOOTTARGET_EXIT
/** \brief Exit point of function EcuM_ASR32_GetBootTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_GETBOOTTARGET_EXIT(Retval,target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_SELECTBOOTTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR40_SelectBootTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_SELECTBOOTTARGET_ENTRY(target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_SELECTBOOTTARGET_EXIT
/** \brief Exit point of function EcuM_ASR40_SelectBootTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_SELECTBOOTTARGET_EXIT(Retval,target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETBOOTTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR40_GetBootTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_GETBOOTTARGET_ENTRY(target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETBOOTTARGET_EXIT
/** \brief Exit point of function EcuM_ASR40_GetBootTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_GETBOOTTARGET_EXIT(Retval,target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function EcuM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GETVERSIONINFO_ENTRY(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETVERSIONINFO_EXIT
/** \brief Exit point of function EcuM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GETVERSIONINFO_EXIT(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GORUN_ENTRY
/** \brief Entry point of function EcuM_GoRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GORUN_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GORUN_EXIT
/** \brief Exit point of function EcuM_GoRun()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GORUN_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GODOWN_ENTRY
/** \brief Entry point of function EcuM_GoDown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GODOWN_ENTRY(caller)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GODOWN_EXIT
/** \brief Exit point of function EcuM_GoDown()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GODOWN_EXIT(Retval,caller)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GOHALT_ENTRY
/** \brief Entry point of function EcuM_GoHalt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GOHALT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GOHALT_EXIT
/** \brief Exit point of function EcuM_GoHalt()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GOHALT_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GOPOLL_ENTRY
/** \brief Entry point of function EcuM_GoPoll()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GOPOLL_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GOPOLL_EXIT
/** \brief Exit point of function EcuM_GoPoll()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GOPOLL_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_INIT_ENTRY
/** \brief Entry point of function EcuM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_INIT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_INIT_EXIT
/** \brief Exit point of function EcuM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_INIT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_STARTUPTWO_ENTRY
/** \brief Entry point of function EcuM_StartupTwo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_STARTUPTWO_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_STARTUPTWO_EXIT
/** \brief Exit point of function EcuM_StartupTwo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_STARTUPTWO_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_SHUTDOWN_ENTRY
/** \brief Entry point of function EcuM_Shutdown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_SHUTDOWN_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_SHUTDOWN_EXIT
/** \brief Exit point of function EcuM_Shutdown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_SHUTDOWN_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_SELECTSHUTDOWNTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR32_SelectShutdownTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_SELECTSHUTDOWNTARGET_ENTRY(target,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_SELECTSHUTDOWNTARGET_EXIT
/** \brief Exit point of function EcuM_ASR32_SelectShutdownTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_SELECTSHUTDOWNTARGET_EXIT(Retval,target,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_GETSHUTDOWNTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR32_GetShutdownTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_GETSHUTDOWNTARGET_ENTRY(shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_GETSHUTDOWNTARGET_EXIT
/** \brief Exit point of function EcuM_ASR32_GetShutdownTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_GETSHUTDOWNTARGET_EXIT(Retval,shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_GETLASTSHUTDOWNTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR32_GetLastShutdownTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_GETLASTSHUTDOWNTARGET_ENTRY(shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_GETLASTSHUTDOWNTARGET_EXIT
/** \brief Exit point of function EcuM_ASR32_GetLastShutdownTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_GETLASTSHUTDOWNTARGET_EXIT(Retval,shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_SELECTSHUTDOWNTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR40_SelectShutdownTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_SELECTSHUTDOWNTARGET_ENTRY(target,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_SELECTSHUTDOWNTARGET_EXIT
/** \brief Exit point of function EcuM_ASR40_SelectShutdownTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_SELECTSHUTDOWNTARGET_EXIT(Retval,target,mode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETSHUTDOWNTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR40_GetShutdownTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_GETSHUTDOWNTARGET_ENTRY(shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETSHUTDOWNTARGET_EXIT
/** \brief Exit point of function EcuM_ASR40_GetShutdownTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_GETSHUTDOWNTARGET_EXIT(Retval,shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETLASTSHUTDOWNTARGET_ENTRY
/** \brief Entry point of function EcuM_ASR40_GetLastShutdownTarget()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_GETLASTSHUTDOWNTARGET_ENTRY(shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETLASTSHUTDOWNTARGET_EXIT
/** \brief Exit point of function EcuM_ASR40_GetLastShutdownTarget()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_GETLASTSHUTDOWNTARGET_EXIT(Retval,shutdownTarget,sleepMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_SELECTSHUTDOWNCAUSE_ENTRY
/** \brief Entry point of function EcuM_ASR40_SelectShutdownCause()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_SELECTSHUTDOWNCAUSE_ENTRY(target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_SELECTSHUTDOWNCAUSE_EXIT
/** \brief Exit point of function EcuM_ASR40_SelectShutdownCause()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_SELECTSHUTDOWNCAUSE_EXIT(Retval,target)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETSHUTDOWNCAUSE_ENTRY
/** \brief Entry point of function EcuM_ASR40_GetShutdownCause()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_GETSHUTDOWNCAUSE_ENTRY(shutdownCause)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETSHUTDOWNCAUSE_EXIT
/** \brief Exit point of function EcuM_ASR40_GetShutdownCause()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_GETSHUTDOWNCAUSE_EXIT(Retval,shutdownCause)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETMOSTRECENTSHUTDOWN_ENTRY
/** \brief Entry point of function EcuM_ASR40_GetMostRecentShutdown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_GETMOSTRECENTSHUTDOWN_ENTRY(target,mode,cause,time)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETMOSTRECENTSHUTDOWN_EXIT
/** \brief Exit point of function EcuM_ASR40_GetMostRecentShutdown()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_GETMOSTRECENTSHUTDOWN_EXIT(Retval,target,mode,cause,time)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETNEXTRECENTSHUTDOWN_ENTRY
/** \brief Entry point of function EcuM_ASR40_GetNextRecentShutdown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_GETNEXTRECENTSHUTDOWN_ENTRY(target,mode,cause,time)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_GETNEXTRECENTSHUTDOWN_EXIT
/** \brief Exit point of function EcuM_ASR40_GetNextRecentShutdown()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_GETNEXTRECENTSHUTDOWN_EXIT(Retval,target,mode,cause,time)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETPENDINGWAKEUPEVENTS_ENTRY
/** \brief Entry point of function EcuM_GetPendingWakeupEvents()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GETPENDINGWAKEUPEVENTS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETPENDINGWAKEUPEVENTS_EXIT
/** \brief Exit point of function EcuM_GetPendingWakeupEvents()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GETPENDINGWAKEUPEVENTS_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CLEARWAKEUPEVENT_ENTRY
/** \brief Entry point of function EcuM_ClearWakeupEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CLEARWAKEUPEVENT_ENTRY(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CLEARWAKEUPEVENT_EXIT
/** \brief Exit point of function EcuM_ClearWakeupEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CLEARWAKEUPEVENT_EXIT(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETVALIDATEDWAKEUPEVENTS_ENTRY
/** \brief Entry point of function EcuM_GetValidatedWakeupEvents()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GETVALIDATEDWAKEUPEVENTS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETVALIDATEDWAKEUPEVENTS_EXIT
/** \brief Exit point of function EcuM_GetValidatedWakeupEvents()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GETVALIDATEDWAKEUPEVENTS_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETEXPIREDWAKEUPEVENTS_ENTRY
/** \brief Entry point of function EcuM_GetExpiredWakeupEvents()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GETEXPIREDWAKEUPEVENTS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETEXPIREDWAKEUPEVENTS_EXIT
/** \brief Exit point of function EcuM_GetExpiredWakeupEvents()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GETEXPIREDWAKEUPEVENTS_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_MAINFUNCTION_ENTRY
/** \brief Entry point of function EcuM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_MAINFUNCTION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_MAINFUNCTION_EXIT
/** \brief Exit point of function EcuM_MainFunction()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_MAINFUNCTION_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_KILLALLRUNREQUESTS_ENTRY
/** \brief Entry point of function EcuM_KillAllRUNRequests()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_KILLALLRUNREQUESTS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_KILLALLRUNREQUESTS_EXIT
/** \brief Exit point of function EcuM_KillAllRUNRequests()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_KILLALLRUNREQUESTS_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETSTATUSOFWAKEUPSOURCE_ENTRY
/** \brief Entry point of function EcuM_GetStatusOfWakeupSource()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GETSTATUSOFWAKEUPSOURCE_ENTRY(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GETSTATUSOFWAKEUPSOURCE_EXIT
/** \brief Exit point of function EcuM_GetStatusOfWakeupSource()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_GETSTATUSOFWAKEUPSOURCE_EXIT(Retval,sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_REQUESTRUN_ENTRY
/** \brief Entry point of function EcuM_ASR40_RequestRUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_REQUESTRUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_REQUESTRUN_EXIT
/** \brief Exit point of function EcuM_ASR40_RequestRUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_REQUESTRUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_RELEASERUN_ENTRY
/** \brief Entry point of function EcuM_ASR40_ReleaseRUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_RELEASERUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_RELEASERUN_EXIT
/** \brief Exit point of function EcuM_ASR40_ReleaseRUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_RELEASERUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_REQUESTPOST_RUN_ENTRY
/** \brief Entry point of function EcuM_ASR40_RequestPOST_RUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_REQUESTPOST_RUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_REQUESTPOST_RUN_EXIT
/** \brief Exit point of function EcuM_ASR40_RequestPOST_RUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_REQUESTPOST_RUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_RELEASEPOST_RUN_ENTRY
/** \brief Entry point of function EcuM_ASR40_ReleasePOST_RUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR40_RELEASEPOST_RUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR40_RELEASEPOST_RUN_EXIT
/** \brief Exit point of function EcuM_ASR40_ReleasePOST_RUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR40_RELEASEPOST_RUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_REQUESTRUN_ENTRY
/** \brief Entry point of function EcuM_ASR32_RequestRUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_REQUESTRUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_REQUESTRUN_EXIT
/** \brief Exit point of function EcuM_ASR32_RequestRUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_REQUESTRUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_RELEASERUN_ENTRY
/** \brief Entry point of function EcuM_ASR32_ReleaseRUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_RELEASERUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_RELEASERUN_EXIT
/** \brief Exit point of function EcuM_ASR32_ReleaseRUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_RELEASERUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_REQUESTPOST_RUN_ENTRY
/** \brief Entry point of function EcuM_ASR32_RequestPOST_RUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_REQUESTPOST_RUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_REQUESTPOST_RUN_EXIT
/** \brief Exit point of function EcuM_ASR32_RequestPOST_RUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_REQUESTPOST_RUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_RELEASEPOST_RUN_ENTRY
/** \brief Entry point of function EcuM_ASR32_ReleasePOST_RUN()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ASR32_RELEASEPOST_RUN_ENTRY(user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ASR32_RELEASEPOST_RUN_EXIT
/** \brief Exit point of function EcuM_ASR32_ReleasePOST_RUN()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_ASR32_RELEASEPOST_RUN_EXIT(Retval,user)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_SETWAKEUPEVENT_ENTRY
/** \brief Entry point of function EcuM_SetWakeupEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_SETWAKEUPEVENT_ENTRY(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_SETWAKEUPEVENT_EXIT
/** \brief Exit point of function EcuM_SetWakeupEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_SETWAKEUPEVENT_EXIT(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_VALIDATEWAKEUPEVENT_ENTRY
/** \brief Entry point of function EcuM_ValidateWakeupEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_VALIDATEWAKEUPEVENT_ENTRY(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_VALIDATEWAKEUPEVENT_EXIT
/** \brief Exit point of function EcuM_ValidateWakeupEvent()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_VALIDATEWAKEUPEVENT_EXIT(sources)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ERRORHOOK_ENTRY
/** \brief Entry point of function EcuM_ErrorHook()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ERRORHOOK_ENTRY(reason)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ERRORHOOK_EXIT
/** \brief Exit point of function EcuM_ErrorHook()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ERRORHOOK_EXIT(reason)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_SETPROGRAMMABLEINTERRUPTS_ENTRY
/** \brief Entry point of function EcuM_AL_SetProgrammableInterrupts()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_SETPROGRAMMABLEINTERRUPTS_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_SETPROGRAMMABLEINTERRUPTS_EXIT
/** \brief Exit point of function EcuM_AL_SetProgrammableInterrupts()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_SETPROGRAMMABLEINTERRUPTS_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_DRIVERINITZERO_ENTRY
/** \brief Entry point of function EcuM_AL_DriverInitZero()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_DRIVERINITZERO_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_DRIVERINITZERO_EXIT
/** \brief Exit point of function EcuM_AL_DriverInitZero()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_DRIVERINITZERO_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DEFAULTINITLISTZERO_ENTRY
/** \brief Entry point of function EcuM_DefaultInitListZero()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DEFAULTINITLISTZERO_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DEFAULTINITLISTZERO_EXIT
/** \brief Exit point of function EcuM_DefaultInitListZero()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DEFAULTINITLISTZERO_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DETERMINEPBCONFIGURATION_ENTRY
/** \brief Entry point of function EcuM_DeterminePbConfiguration()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DETERMINEPBCONFIGURATION_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DETERMINEPBCONFIGURATION_EXIT
/** \brief Exit point of function EcuM_DeterminePbConfiguration()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_DETERMINEPBCONFIGURATION_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_DRIVERINITONE_ENTRY
/** \brief Entry point of function EcuM_AL_DriverInitOne()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_DRIVERINITONE_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_DRIVERINITONE_EXIT
/** \brief Exit point of function EcuM_AL_DriverInitOne()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_DRIVERINITONE_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DEFAULTINITLISTONE_ENTRY
/** \brief Entry point of function EcuM_DefaultInitListOne()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DEFAULTINITLISTONE_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DEFAULTINITLISTONE_EXIT
/** \brief Exit point of function EcuM_DefaultInitListOne()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DEFAULTINITLISTONE_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONENTERRUN_ENTRY
/** \brief Entry point of function EcuM_OnEnterRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONENTERRUN_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONENTERRUN_EXIT
/** \brief Exit point of function EcuM_OnEnterRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONENTERRUN_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONEXITRUN_ENTRY
/** \brief Entry point of function EcuM_OnExitRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONEXITRUN_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONEXITRUN_EXIT
/** \brief Exit point of function EcuM_OnExitRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONEXITRUN_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONEXITPOSTRUN_ENTRY
/** \brief Entry point of function EcuM_OnExitPostRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONEXITPOSTRUN_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONEXITPOSTRUN_EXIT
/** \brief Exit point of function EcuM_OnExitPostRun()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONEXITPOSTRUN_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONRTESTARTUP_ENTRY
/** \brief Entry point of function EcuM_OnRTEStartup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONRTESTARTUP_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONRTESTARTUP_EXIT
/** \brief Exit point of function EcuM_OnRTEStartup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONRTESTARTUP_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONGOOFFONE_ENTRY
/** \brief Entry point of function EcuM_OnGoOffOne()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONGOOFFONE_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONGOOFFONE_EXIT
/** \brief Exit point of function EcuM_OnGoOffOne()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONGOOFFONE_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONGOOFFTWO_ENTRY
/** \brief Entry point of function EcuM_OnGoOffTwo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONGOOFFTWO_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONGOOFFTWO_EXIT
/** \brief Exit point of function EcuM_OnGoOffTwo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONGOOFFTWO_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONPREPSHUTDOWN_ENTRY
/** \brief Entry point of function EcuM_OnPrepShutdown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONPREPSHUTDOWN_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONPREPSHUTDOWN_EXIT
/** \brief Exit point of function EcuM_OnPrepShutdown()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONPREPSHUTDOWN_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ENABLEWAKEUPSOURCES_ENTRY
/** \brief Entry point of function EcuM_EnableWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ENABLEWAKEUPSOURCES_ENTRY(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ENABLEWAKEUPSOURCES_EXIT
/** \brief Exit point of function EcuM_EnableWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ENABLEWAKEUPSOURCES_EXIT(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GENERATERAMHASH_ENTRY
/** \brief Entry point of function EcuM_GenerateRamHash()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GENERATERAMHASH_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_GENERATERAMHASH_EXIT
/** \brief Exit point of function EcuM_GenerateRamHash()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_GENERATERAMHASH_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_SWITCHOFF_ENTRY
/** \brief Entry point of function EcuM_AL_SwitchOff()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_SWITCHOFF_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_SWITCHOFF_EXIT
/** \brief Exit point of function EcuM_AL_SwitchOff()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_SWITCHOFF_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_RESET_ENTRY
/** \brief Entry point of function EcuM_AL_Reset()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_RESET_ENTRY(reset)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_RESET_EXIT
/** \brief Exit point of function EcuM_AL_Reset()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_RESET_EXIT(reset)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CHECKRAMHASH_ENTRY
/** \brief Entry point of function EcuM_CheckRamHash()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CHECKRAMHASH_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CHECKRAMHASH_EXIT
/** \brief Exit point of function EcuM_CheckRamHash()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_ECUM_CHECKRAMHASH_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DISABLEWAKEUPSOURCES_ENTRY
/** \brief Entry point of function EcuM_DisableWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DISABLEWAKEUPSOURCES_ENTRY(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DISABLEWAKEUPSOURCES_EXIT
/** \brief Exit point of function EcuM_DisableWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DISABLEWAKEUPSOURCES_EXIT(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_DRIVERRESTART_ENTRY
/** \brief Entry point of function EcuM_AL_DriverRestart()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_DRIVERRESTART_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_AL_DRIVERRESTART_EXIT
/** \brief Exit point of function EcuM_AL_DriverRestart()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_AL_DRIVERRESTART_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DEFAULTRESTARTLIST_ENTRY
/** \brief Entry point of function EcuM_DefaultRestartList()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DEFAULTRESTARTLIST_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_DEFAULTRESTARTLIST_EXIT
/** \brief Exit point of function EcuM_DefaultRestartList()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_DEFAULTRESTARTLIST_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_STARTWAKEUPSOURCES_ENTRY
/** \brief Entry point of function EcuM_StartWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_STARTWAKEUPSOURCES_ENTRY(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_STARTWAKEUPSOURCES_EXIT
/** \brief Exit point of function EcuM_StartWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_STARTWAKEUPSOURCES_EXIT(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CHECKVALIDATION_ENTRY
/** \brief Entry point of function EcuM_CheckValidation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CHECKVALIDATION_ENTRY(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CHECKVALIDATION_EXIT
/** \brief Exit point of function EcuM_CheckValidation()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CHECKVALIDATION_EXIT(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_STOPWAKEUPSOURCES_ENTRY
/** \brief Entry point of function EcuM_StopWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_STOPWAKEUPSOURCES_ENTRY(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_STOPWAKEUPSOURCES_EXIT
/** \brief Exit point of function EcuM_StopWakeupSources()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_STOPWAKEUPSOURCES_EXIT(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_SLEEPACTIVITY_ENTRY
/** \brief Entry point of function EcuM_SleepActivity()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_SLEEPACTIVITY_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_SLEEPACTIVITY_EXIT
/** \brief Exit point of function EcuM_SleepActivity()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_SLEEPACTIVITY_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CHECKWAKEUP_ENTRY
/** \brief Entry point of function EcuM_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CHECKWAKEUP_ENTRY(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_CHECKWAKEUP_EXIT
/** \brief Exit point of function EcuM_CheckWakeup()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_CHECKWAKEUP_EXIT(wakeupSource)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_PREHALT_ENTRY
/** \brief Entry point of function EcuM_PreHalt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_PREHALT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_PREHALT_EXIT
/** \brief Exit point of function EcuM_PreHalt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_PREHALT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_POSTHALT_ENTRY
/** \brief Entry point of function EcuM_PostHalt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_POSTHALT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_POSTHALT_EXIT
/** \brief Exit point of function EcuM_PostHalt()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_POSTHALT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONGOSLEEP_ENTRY
/** \brief Entry point of function EcuM_OnGoSleep()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONGOSLEEP_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_ECUM_ONGOSLEEP_EXIT
/** \brief Exit point of function EcuM_OnGoSleep()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_ECUM_ONGOSLEEP_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SHUTDOWNHOOK_ENTRY
/** \brief Entry point of function ShutdownHook()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SHUTDOWNHOOK_ENTRY(errorID)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_SHUTDOWNHOOK_EXIT
/** \brief Exit point of function ShutdownHook()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_SHUTDOWNHOOK_EXIT(errorID)
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

#endif /* ifndef ECUM_TRACE_H */
/*==================[end of file]===========================================*/

