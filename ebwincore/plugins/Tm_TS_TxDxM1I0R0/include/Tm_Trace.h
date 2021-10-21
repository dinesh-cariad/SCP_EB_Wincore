/**
 * \file
 *
 * \brief AUTOSAR Tm
 *
 * This file contains the implementation of the AUTOSAR
 * module Tm.
 *
 * \version 1.0.4
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef TM_TRACE_H
#define TM_TRACE_H

/*==[Includes]================================================================*/


#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif

/*==[Macros]==================================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function Tm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_GETVERSIONINFO_ENTRY(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETVERSIONINFO_EXIT
/** \brief Exit point of function Tm_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_GETVERSIONINFO_EXIT(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER1US16BIT_ENTRY
/** \brief Entry point of function Tm_ResetTimer1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_RESETTIMER1US16BIT_ENTRY(TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER1US16BIT_EXIT
/** \brief Exit point of function Tm_ResetTimer1us16bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_RESETTIMER1US16BIT_EXIT(RetVal,TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN1US16BIT_ENTRY
/** \brief Entry point of function Tm_GetTimeSpan1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_GETTIMESPAN1US16BIT_ENTRY(TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN1US16BIT_EXIT
/** \brief Exit point of function Tm_GetTimeSpan1us16bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_GETTIMESPAN1US16BIT_EXIT(RetVal,TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER1US16BIT_ENTRY
/** \brief Entry point of function Tm_ShiftTimer1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER1US16BIT_ENTRY(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER1US16BIT_EXIT
/** \brief Exit point of function Tm_ShiftTimer1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER1US16BIT_EXIT(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER1US16BIT_ENTRY
/** \brief Entry point of function Tm_SyncTimer1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER1US16BIT_ENTRY(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER1US16BIT_EXIT
/** \brief Exit point of function Tm_SyncTimer1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER1US16BIT_EXIT(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_BUSYWAIT1US16BIT_ENTRY
/** \brief Entry point of function Tm_BusyWait1us16bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_BUSYWAIT1US16BIT_ENTRY(WaitingTimeMin)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_BUSYWAIT1US16BIT_EXIT
/** \brief Exit point of function Tm_BusyWait1us16bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_BUSYWAIT1US16BIT_EXIT(RetVal,WaitingTimeMin)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER1US24BIT_ENTRY
/** \brief Entry point of function Tm_ResetTimer1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_RESETTIMER1US24BIT_ENTRY(TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER1US24BIT_EXIT
/** \brief Exit point of function Tm_ResetTimer1us24bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_RESETTIMER1US24BIT_EXIT(RetVal,TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN1US24BIT_ENTRY
/** \brief Entry point of function Tm_GetTimeSpan1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_GETTIMESPAN1US24BIT_ENTRY(TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN1US24BIT_EXIT
/** \brief Exit point of function Tm_GetTimeSpan1us24bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_GETTIMESPAN1US24BIT_EXIT(RetVal,TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER1US24BIT_ENTRY
/** \brief Entry point of function Tm_ShiftTimer1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER1US24BIT_ENTRY(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER1US24BIT_EXIT
/** \brief Exit point of function Tm_ShiftTimer1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER1US24BIT_EXIT(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER1US24BIT_ENTRY
/** \brief Entry point of function Tm_SyncTimer1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER1US24BIT_ENTRY(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER1US24BIT_EXIT
/** \brief Exit point of function Tm_SyncTimer1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER1US24BIT_EXIT(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_BUSYWAIT1US24BIT_ENTRY
/** \brief Entry point of function Tm_BusyWait1us24bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_BUSYWAIT1US24BIT_ENTRY(WaitingTimeMin)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_BUSYWAIT1US24BIT_EXIT
/** \brief Exit point of function Tm_BusyWait1us24bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_BUSYWAIT1US24BIT_EXIT(RetVal,WaitingTimeMin)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER1US32BIT_ENTRY
/** \brief Entry point of function Tm_ResetTimer1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_RESETTIMER1US32BIT_ENTRY(TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER1US32BIT_EXIT
/** \brief Exit point of function Tm_ResetTimer1us32bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_RESETTIMER1US32BIT_EXIT(RetVal,TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN1US32BIT_ENTRY
/** \brief Entry point of function Tm_GetTimeSpan1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_GETTIMESPAN1US32BIT_ENTRY(TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN1US32BIT_EXIT
/** \brief Exit point of function Tm_GetTimeSpan1us32bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_GETTIMESPAN1US32BIT_EXIT(RetVal,TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER1US32BIT_ENTRY
/** \brief Entry point of function Tm_ShiftTimer1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER1US32BIT_ENTRY(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER1US32BIT_EXIT
/** \brief Exit point of function Tm_ShiftTimer1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER1US32BIT_EXIT(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER1US32BIT_ENTRY
/** \brief Entry point of function Tm_SyncTimer1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER1US32BIT_ENTRY(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER1US32BIT_EXIT
/** \brief Exit point of function Tm_SyncTimer1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER1US32BIT_EXIT(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_BUSYWAIT1US32BIT_ENTRY
/** \brief Entry point of function Tm_BusyWait1us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_BUSYWAIT1US32BIT_ENTRY(WaitingTimeMin)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_BUSYWAIT1US32BIT_EXIT
/** \brief Exit point of function Tm_BusyWait1us32bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_BUSYWAIT1US32BIT_EXIT(RetVal,WaitingTimeMin)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER100US32BIT_ENTRY
/** \brief Entry point of function Tm_ResetTimer100us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_RESETTIMER100US32BIT_ENTRY(TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_RESETTIMER100US32BIT_EXIT
/** \brief Exit point of function Tm_ResetTimer100us32bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_RESETTIMER100US32BIT_EXIT(RetVal,TimerPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN100US32BIT_ENTRY
/** \brief Entry point of function Tm_GetTimeSpan100us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_GETTIMESPAN100US32BIT_ENTRY(TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_GETTIMESPAN100US32BIT_EXIT
/** \brief Exit point of function Tm_GetTimeSpan100us32bit()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_TM_GETTIMESPAN100US32BIT_EXIT(RetVal,TimerPtr,TimeSpanPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER100US32BIT_ENTRY
/** \brief Entry point of function Tm_ShiftTimer100us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER100US32BIT_ENTRY(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SHIFTTIMER100US32BIT_EXIT
/** \brief Exit point of function Tm_ShiftTimer100us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SHIFTTIMER100US32BIT_EXIT(TimerPtr,TimeValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER100US32BIT_ENTRY
/** \brief Entry point of function Tm_SyncTimer100us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER100US32BIT_ENTRY(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_TM_SYNCTIMER100US32BIT_EXIT
/** \brief Exit point of function Tm_SyncTimer100us32bit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_TM_SYNCTIMER100US32BIT_EXIT(TimerDstPtr,TimerSrcPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Declaration of functions with external linkage]==========================*/

#endif /* ifndef TM_TRACE_H */

