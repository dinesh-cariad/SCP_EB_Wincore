/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef COMM_TRACE_H
#define COMM_TRACE_H
/*==================[inclusions]============================================*/

#include <Std_Types.h>

/* Check if BASE_DBG_ENABLE is defined and set to STD_ON.
 * If so, include Base_Dbg.h which includes a custom header file */
#if (defined BASE_DBG_ENABLE)
#if (BASE_DBG_ENABLE == STD_ON)

#include <Base_Dbg.h>

#endif
#endif
#include <ComM_Trace_Internal.h>

/*==================[macros]================================================*/

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_INIT_ENTRY
/** \brief Entry point of function ComM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_INIT_ENTRY(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_INIT_EXIT
/** \brief Exit point of function ComM_Init()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_INIT_EXIT(ConfigPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_DEINIT_ENTRY
/** \brief Entry point of function ComM_DeInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_DEINIT_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_DEINIT_EXIT
/** \brief Exit point of function ComM_DeInit()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_DEINIT_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_GETSTATE_ENTRY
/** \brief Entry point of function ComM_GetState()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_GETSTATE_ENTRY(Channel,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_GETSTATE_EXIT
/** \brief Exit point of function ComM_GetState()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_GETSTATE_EXIT(Retval,Channel,State)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_GETSTATUS_ENTRY
/** \brief Entry point of function ComM_GetStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_GETSTATUS_ENTRY(Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_GETSTATUS_EXIT
/** \brief Exit point of function ComM_GetStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_GETSTATUS_EXIT(Retval,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETINHIBITIONSTATUS_ENTRY
/** \brief Entry point of function ComM_ASR40_GetInhibitionStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_GETINHIBITIONSTATUS_ENTRY(Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETINHIBITIONSTATUS_EXIT
/** \brief Exit point of function ComM_ASR40_GetInhibitionStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_GETINHIBITIONSTATUS_EXIT(Retval,Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_RequestComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_REQUESTCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_REQUESTCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_RequestComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_REQUESTCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETMAXCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_GetMaxComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_GETMAXCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETMAXCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_GetMaxComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_GETMAXCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETREQUESTEDCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_GetRequestedComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_GETREQUESTEDCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETREQUESTEDCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_GetRequestedComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_GETREQUESTEDCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETCURRENTCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_GetCurrentComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_GETCURRENTCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETCURRENTCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_GetCurrentComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_GETCURRENTCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETCURRENTPNCCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_GetCurrentPNCComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_GETCURRENTPNCCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_GETCURRENTPNCCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_GetCurrentPNCComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_GETCURRENTPNCCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_PREVENTWAKEUP_ENTRY
/** \brief Entry point of function ComM_ASR40_PreventWakeUp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_PREVENTWAKEUP_ENTRY(Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_PREVENTWAKEUP_EXIT
/** \brief Exit point of function ComM_ASR40_PreventWakeUp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_PREVENTWAKEUP_EXIT(Retval,Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_LIMITCHANNELTONOCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_LimitChannelToNoComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_LIMITCHANNELTONOCOMMODE_ENTRY(Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_LIMITCHANNELTONOCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_LimitChannelToNoComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_LIMITCHANNELTONOCOMMODE_EXIT(Retval,Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_LIMITECUTONOCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR40_LimitECUToNoComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_LIMITECUTONOCOMMODE_ENTRY(Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_LIMITECUTONOCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR40_LimitECUToNoComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_LIMITECUTONOCOMMODE_EXIT(Retval,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_READINHIBITCOUNTER_ENTRY
/** \brief Entry point of function ComM_ASR40_ReadInhibitCounter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_READINHIBITCOUNTER_ENTRY(CounterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_READINHIBITCOUNTER_EXIT
/** \brief Exit point of function ComM_ASR40_ReadInhibitCounter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_READINHIBITCOUNTER_EXIT(Retval,CounterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_RESETINHIBITCOUNTER_ENTRY
/** \brief Entry point of function ComM_ASR40_ResetInhibitCounter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_RESETINHIBITCOUNTER_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_RESETINHIBITCOUNTER_EXIT
/** \brief Exit point of function ComM_ASR40_ResetInhibitCounter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_RESETINHIBITCOUNTER_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_SETECUGROUPCLASSIFICATION_ENTRY
/** \brief Entry point of function ComM_ASR40_SetECUGroupClassification()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR40_SETECUGROUPCLASSIFICATION_ENTRY(Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR40_SETECUGROUPCLASSIFICATION_EXIT
/** \brief Exit point of function ComM_ASR40_SetECUGroupClassification()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR40_SETECUGROUPCLASSIFICATION_EXIT(Retval,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETINHIBITIONSTATUS_ENTRY
/** \brief Entry point of function ComM_ASR32_GetInhibitionStatus()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_GETINHIBITIONSTATUS_ENTRY(Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETINHIBITIONSTATUS_EXIT
/** \brief Exit point of function ComM_ASR32_GetInhibitionStatus()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_GETINHIBITIONSTATUS_EXIT(Retval,Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_REQUESTCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR32_RequestComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_REQUESTCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_REQUESTCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR32_RequestComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_REQUESTCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETMAXCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR32_GetMaxComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_GETMAXCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETMAXCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR32_GetMaxComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_GETMAXCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETREQUESTEDMODE_ENTRY
/** \brief Entry point of function ComM_ASR32_GetRequestedMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_GETREQUESTEDMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETREQUESTEDMODE_EXIT
/** \brief Exit point of function ComM_ASR32_GetRequestedMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_GETREQUESTEDMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETCURRENTCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR32_GetCurrentComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_GETCURRENTCOMMODE_ENTRY(User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_GETCURRENTCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR32_GetCurrentComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_GETCURRENTCOMMODE_EXIT(Retval,User,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_PREVENTWAKEUP_ENTRY
/** \brief Entry point of function ComM_ASR32_PreventWakeUp()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_PREVENTWAKEUP_ENTRY(Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_PREVENTWAKEUP_EXIT
/** \brief Exit point of function ComM_ASR32_PreventWakeUp()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_PREVENTWAKEUP_EXIT(Retval,Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_LIMITCHANNELTONOCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR32_LimitChannelToNoComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_LIMITCHANNELTONOCOMMODE_ENTRY(Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_LIMITCHANNELTONOCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR32_LimitChannelToNoComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_LIMITCHANNELTONOCOMMODE_EXIT(Retval,Channel,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_LIMITECUTONOCOMMODE_ENTRY
/** \brief Entry point of function ComM_ASR32_LimitECUToNoComMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_LIMITECUTONOCOMMODE_ENTRY(Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_LIMITECUTONOCOMMODE_EXIT
/** \brief Exit point of function ComM_ASR32_LimitECUToNoComMode()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_LIMITECUTONOCOMMODE_EXIT(Retval,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_READINHIBITCOUNTER_ENTRY
/** \brief Entry point of function ComM_ASR32_ReadInhibitCounter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_READINHIBITCOUNTER_ENTRY(CounterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_READINHIBITCOUNTER_EXIT
/** \brief Exit point of function ComM_ASR32_ReadInhibitCounter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_READINHIBITCOUNTER_EXIT(Retval,CounterValue)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_RESETINHIBITCOUNTER_ENTRY
/** \brief Entry point of function ComM_ASR32_ResetInhibitCounter()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_RESETINHIBITCOUNTER_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_RESETINHIBITCOUNTER_EXIT
/** \brief Exit point of function ComM_ASR32_ResetInhibitCounter()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_RESETINHIBITCOUNTER_EXIT(Retval)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_SETECUGROUPCLASSIFICATION_ENTRY
/** \brief Entry point of function ComM_ASR32_SetECUGroupClassification()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ASR32_SETECUGROUPCLASSIFICATION_ENTRY(Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ASR32_SETECUGROUPCLASSIFICATION_EXIT
/** \brief Exit point of function ComM_ASR32_SetECUGroupClassification()
 **
 ** The first parameter of the macro is the return
 ** value of the function, followed by the arguments
 ** of the function.
 */
#define DBG_COMM_ASR32_SETECUGROUPCLASSIFICATION_EXIT(Retval,Status)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_GETVERSIONINFO_ENTRY
/** \brief Entry point of function ComM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_GETVERSIONINFO_ENTRY(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_GETVERSIONINFO_EXIT
/** \brief Exit point of function ComM_GetVersionInfo()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_GETVERSIONINFO_EXIT(VersionInfoPtr)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_BUSSM_MODEINDICATION_ENTRY
/** \brief Entry point of function ComM_BusSM_ModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_BUSSM_MODEINDICATION_ENTRY(Channel,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_BUSSM_MODEINDICATION_EXIT
/** \brief Exit point of function ComM_BusSM_ModeIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_BUSSM_MODEINDICATION_EXIT(Channel,ComMode)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_DCM_ACTIVEDIAGNOSTIC_ENTRY
/** \brief Entry point of function ComM_DCM_ActiveDiagnostic()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_DCM_ACTIVEDIAGNOSTIC_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_DCM_ACTIVEDIAGNOSTIC_EXIT
/** \brief Exit point of function ComM_DCM_ActiveDiagnostic()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_DCM_ACTIVEDIAGNOSTIC_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_DCM_INACTIVEDIAGNOSTIC_ENTRY
/** \brief Entry point of function ComM_DCM_InactiveDiagnostic()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_DCM_INACTIVEDIAGNOSTIC_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_DCM_INACTIVEDIAGNOSTIC_EXIT
/** \brief Exit point of function ComM_DCM_InactiveDiagnostic()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_DCM_INACTIVEDIAGNOSTIC_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_MAINFUNCTION_XXX_ENTRY
/** \brief Entry point of function ComM_MainFunction_XXX()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_MAINFUNCTION_XXX_ENTRY()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_MAINFUNCTION_XXX_EXIT
/** \brief Exit point of function ComM_MainFunction_XXX()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_MAINFUNCTION_XXX_EXIT()
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ECUM_WAKEUPINDICATION_ENTRY
/** \brief Entry point of function ComM_EcuM_WakeUpIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ECUM_WAKEUPINDICATION_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ECUM_WAKEUPINDICATION_EXIT
/** \brief Exit point of function ComM_EcuM_WakeUpIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ECUM_WAKEUPINDICATION_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ECUM_PNCWAKEUPINDICATION_ENTRY
/** \brief Entry point of function ComM_EcuM_PNCWakeUpIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ECUM_PNCWAKEUPINDICATION_ENTRY(PNCid)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_ECUM_PNCWAKEUPINDICATION_EXIT
/** \brief Exit point of function ComM_EcuM_PNCWakeUpIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_ECUM_PNCWAKEUPINDICATION_EXIT(PNCid)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_COMMUNICATIONALLOWED_ENTRY
/** \brief Entry point of function ComM_CommunicationAllowed()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_COMMUNICATIONALLOWED_ENTRY(Channel,Allowed)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_COMMUNICATIONALLOWED_EXIT
/** \brief Exit point of function ComM_CommunicationAllowed()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_COMMUNICATIONALLOWED_EXIT(Channel,Allowed)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_NETWORKSTARTINDICATION_ENTRY
/** \brief Entry point of function ComM_Nm_NetworkStartIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_NETWORKSTARTINDICATION_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_NETWORKSTARTINDICATION_EXIT
/** \brief Exit point of function ComM_Nm_NetworkStartIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_NETWORKSTARTINDICATION_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_NETWORKMODE_ENTRY
/** \brief Entry point of function ComM_Nm_NetworkMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_NETWORKMODE_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_NETWORKMODE_EXIT
/** \brief Exit point of function ComM_Nm_NetworkMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_NETWORKMODE_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_PREPAREBUSSLEEPMODE_ENTRY
/** \brief Entry point of function ComM_Nm_PrepareBusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_PREPAREBUSSLEEPMODE_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_PREPAREBUSSLEEPMODE_EXIT
/** \brief Exit point of function ComM_Nm_PrepareBusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_PREPAREBUSSLEEPMODE_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_BUSSLEEPMODE_ENTRY
/** \brief Entry point of function ComM_Nm_BusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_BUSSLEEPMODE_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_BUSSLEEPMODE_EXIT
/** \brief Exit point of function ComM_Nm_BusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_BUSSLEEPMODE_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_RESTARTINDICATION_ENTRY
/** \brief Entry point of function ComM_Nm_RestartIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_RESTARTINDICATION_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_NM_RESTARTINDICATION_EXIT
/** \brief Exit point of function ComM_Nm_RestartIndication()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_NM_RESTARTINDICATION_EXIT(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */


/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_BUSSM_BUSSLEEPMODE_ENTRY
/** \brief Entry point of function ComM_BusSM_BusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_BUSSM_BUSSLEEPMODE_ENTRY(Channel)
#endif
/** \brief Exclude section from doxygen
 ** @endcond */

/** @cond
 ** \brief Exclude section from doxygen */
#ifndef DBG_COMM_BUSSM_BUSSLEEPMODE_EXIT
/** \brief Exit point of function ComM_BusSM_BusSleepMode()
 **
 ** The macro takes the function arguments as parameters.
 */
#define DBG_COMM_BUSSM_BUSSLEEPMODE_EXIT(Channel)
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

#endif /* ifndef COMM_TRACE_H */
/*==================[end of file]===========================================*/
