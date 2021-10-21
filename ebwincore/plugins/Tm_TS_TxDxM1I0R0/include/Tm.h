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

#ifndef TM_H
#define TM_H

/*==[Includes]================================================================*/

/* !LINKSTO Tm.swdd.Std_ReturnType,1, Tm.swdd.Std_VersionInfoType,1 */
#include <Std_Types.h>

#include <Tm_Types.h>

#include <Tm_Version.h>

/*==[Macros]==================================================================*/

#if (defined TM_SID_GET_VERSION_INFO)
#error TM_SID_GET_VERSION_INFO already defined
#endif
/** \brief Service ID for Tm_GetVersionInfo. */
#define TM_SID_GET_VERSION_INFO 0x1

#if (defined TM_SID_RESET_TIMER_1US_16BIT)
#error TM_SID_RESET_TIMER_1US_16BIT already defined
#endif
/** \brief Service ID for Tm_ResetTimer1us16bit. */
#define TM_SID_RESET_TIMER_1US_16BIT 0x2

#if (defined TM_SID_GET_TIMESPAN_1US_16BIT)
#error TM_SID_GET_TIMESPAN_1US_16BIT already defined
#endif
/** \brief Service ID for Tm_GetTimeSpan1us16bit. */
#define TM_SID_GET_TIMESPAN_1US_16BIT 0x3

#if (defined TM_SID_SHIFT_TIMER_1US_16BIT)
#error TM_SID_SHIFT_TIMER_1US_16BIT already defined
#endif
/** \brief Service ID for Tm_ShiftTimer1us16bit. */
#define TM_SID_SHIFT_TIMER_1US_16BIT 0x4

#if (defined TM_SID_SYNC_TIMER_1US_16BIT)
#error TM_SID_SYNC_TIMER_1US_16BIT already defined
#endif
/** \brief Service ID for Tm_SyncTimer1us16bit. */
#define TM_SID_SYNC_TIMER_1US_16BIT 0x5

#if (defined TM_SID_BUSY_WAIT_1US_16BIT)
#error TM_SID_BUSY_WAIT_1US_16BIT already defined
#endif
/** \brief Service ID for Tm_BusyWait1us16bit. */
#define TM_SID_BUSY_WAIT_1US_16BIT 0x6

#if (defined TM_SID_RESET_TIMER_1US_24BIT)
#error TM_SID_RESET_TIMER_1US_24BIT already defined
#endif
/** \brief Service ID for Tm_ResetTimer1us24bit. */
#define TM_SID_RESET_TIMER_1US_24BIT 0x7

#if (defined TM_SID_GET_TIMESPAN_1US_24BIT)
#error TM_SID_GET_TIMESPAN_1US_24BIT already defined
#endif
/** \brief Service ID for Tm_GetTimeSpan1us24bit. */
#define TM_SID_GET_TIMESPAN_1US_24BIT 0x8

#if (defined TM_SID_SHIFT_TIMER_1US_24BIT)
#error TM_SID_SHIFT_TIMER_1US_24BIT already defined
#endif
/** \brief Service ID for Tm_ShiftTimer1us24bit. */
#define TM_SID_SHIFT_TIMER_1US_24BIT 0x9

#if (defined TM_SID_SYNC_TIMER_1US_24BIT)
#error TM_SID_SYNC_TIMER_1US_24BIT already defined
#endif
/** \brief Service ID for Tm_SyncTimer1us24bit. */
#define TM_SID_SYNC_TIMER_1US_24BIT 0xA

#if (defined TM_SID_BUSY_WAIT_1US_24BIT)
#error TM_SID_BUSY_WAIT_1US_24BIT already defined
#endif
/** \brief Service ID for Tm_BusyWait1us24bit. */
#define TM_SID_BUSY_WAIT_1US_24BIT 0xB

#if (defined TM_SID_RESET_TIMER_1US_32BIT)
#error TM_SID_RESET_TIMER_1US_32BIT already defined
#endif
/** \brief Service ID for Tm_ResetTimer1us32bit. */
#define TM_SID_RESET_TIMER_1US_32BIT 0xC

#if (defined TM_SID_GET_TIMESPAN_1US_32BIT)
#error TM_SID_GET_TIMESPAN_1US_32BIT already defined
#endif
/** \brief Service ID for Tm_GetTimeSpan1us32bit. */
#define TM_SID_GET_TIMESPAN_1US_32BIT 0xD

#if (defined TM_SID_SHIFT_TIMER_1US_32BIT)
#error TM_SID_SHIFT_TIMER_1US_32BIT already defined
#endif
/** \brief Service ID for Tm_ShiftTimer1us32bit. */
#define TM_SID_SHIFT_TIMER_1US_32BIT 0xE

#if (defined TM_SID_SYNC_TIMER_1US_32BIT)
#error TM_SID_SYNC_TIMER_1US_32BIT already defined
#endif
/** \brief Service ID for Tm_SyncTimer1us32bit. */
#define TM_SID_SYNC_TIMER_1US_32BIT 0xF

#if (defined TM_SID_BUSY_WAIT_1US_32BIT)
#error TM_SID_BUSY_WAIT_1US_32BIT already defined
#endif
/** \brief Service ID for Tm_BusyWait1us32bit. */
#define TM_SID_BUSY_WAIT_1US_32BIT 0x10

#if (defined TM_SID_RESET_TIMER_100US_32BIT)
#error TM_SID_RESET_TIMER_100US_32BIT already defined
#endif
/** \brief Service ID for Tm_ResetTimer100us32bit. */
#define TM_SID_RESET_TIMER_100US_32BIT 0x11

#if (defined TM_SID_GET_TIME_SPAN_100US_32BIT)
#error TM_SID_GET_TIME_SPAN_100US_32BIT already defined
#endif
/** \brief Service ID for Tm_GetTimeSpan100us32bit. */
#define TM_SID_GET_TIME_SPAN_100US_32BIT 0x12

#if (defined TM_SID_SHIFT_TIMER_100US_32BIT)
#error TM_SID_SHIFT_TIMER_100US_32BIT already defined
#endif
/** \brief Service ID for Tm_ShiftTimer100us32bit. */
#define TM_SID_SHIFT_TIMER_100US_32BIT 0x13

#if (defined TM_SID_SYNC_TIMER_100US_32BIT)
#error TM_SID_SYNC_TIMER_100US_32BIT already defined
#endif
/** \brief Service ID for Tm_SyncTimer100us32bit. */
#define TM_SID_SYNC_TIMER_100US_32BIT 0x14

#if (defined TM_E_PARAM_POINTER)
#error TM_E_PARAM_POINTER already defined
#endif
/** \brief API parameter checking: invalid pointer */
#define TM_E_PARAM_POINTER  0x01U

#if (defined TM_E_PARAM_VALUE)
#error TM_E_PARAM_VALUE  already defined
#endif
/** \brief API parameter checking: invalid value*/
#define TM_E_PARAM_VALUE    0x02U

#if (defined TM_E_HARDWARE_TIMER)
#error TM_E_HARDWARE_TIMER already defined
#endif
/** \brief Access to underlying hardware timer failed */
#define TM_E_HARDWARE_TIMER 0x03U

#if (defined TM_INSTANCE_ID)
#error TM_INSTANCE_ID already defined
#endif
/** \brief Module's instance ID. */
#define TM_INSTANCE_ID      0U

/*==[Types]===================================================================*/

/*==[Constants with external linkage]=========================================*/

/*==[Variables with external linkage]=========================================*/

/*==[Declaration of functions with external linkage]==========================*/

#define TM_START_SEC_CODE
#include <Tm_MemMap.h>

#if (TM_VERSION_INFO_API == STD_ON)

/** \brief Returns the version information of this module.
 **
 ** This function returns the version information of this module into parameter VersionInfoPtr
 **
 ** \param[out] VersionInfoPtr Pointer to a VersionInfo stucture on wich to store version of this
 ** module
 **
 ** \ServiceID{0x1}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_GetVersionInfo
(
  P2VAR(Std_VersionInfoType, AUTOMATIC, TM_APPL_DATA)             VersionInfoPtr
);

#endif /* #if (TM_VERSION_INFO_API == STD_ON) */

/* !LINKSTO SWS_Tm_00026_0,1, SWS_Tm_00027_0,1 */
#if (TM_ENABLE_PREDEF_TIMER_1US_16BIT == STD_ON)

/** \brief Resets a timer instance (user point of view).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[out] TimerPtr Pointer to a timer instance defined by the user.
 **
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development
 **                   error has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x2}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us16bit
(
  P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr
);

/** \brief Delivers the time difference (current time - reference time).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] TimerPtr Pointer to a timer instance defined by the user.
 ** \param[out] TimeSpanPtr Pointer to time span destination data in RAM
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x3}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us16bit
(
  P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  P2VAR(uint16, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
);

/** \brief Shifts the reference time of the timer instance.
 **
 ** \param[in] TimeValue Time value in μs, the reference time has to be shifted.
 ** \param[in, out] TimerPtr Pointer to a timer instance defined by the user.
 **
 ** \ServiceID{0x4}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_ShiftTimer1us16bit
(
  P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr,
  uint16 TimeValue
);

/** \brief Synchronizes two timer instances.
 **
 ** \param[in] TimerSrcPtr Pointer to the source timer instance defined by the user.
 ** \param[out] TimerDstPtr Pointer to the destination timer instance defined by the user.
 **
 ** \ServiceID{0x5}
 ** \Reentrancy{Reentrant but not for the same destination timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_SyncTimer1us16bit
(
  P2VAR(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)      TimerDstPtr,
  P2CONST(Tm_PredefTimer1us16bitType, AUTOMATIC, TM_APPL_DATA)    TimerSrcPtr
);

/** \brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] WaitingTimeMin Minimum waiting time in microseconds.
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x6}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us16bit
(
  uint8 WaitingTimeMin
);

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_16BIT == STD_ON) */

/* !LINKSTO SWS_Tm_00026_1,1, SWS_Tm_00027_1,1 */
#if (TM_ENABLE_PREDEF_TIMER_1US_24BIT == STD_ON)

/** \brief Resets a timer instance (user point of view).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[out] TimerPtr Pointer to a timer instance defined by the user.
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x7}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us24bit
(
  P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr
);

/** \brief Delivers the time difference (current time - reference time)
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] TimerPtr Pointer to a timer instance defined by the user.
 ** \param[out] TimeSpanPtr Pointer to time span destination data in RAM
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x8}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us24bit
(
  P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  P2VAR(uint32, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
);

/** \brief Shifts the reference time of the timer instance.
 **
 ** \param[in] TimeValue Time value in μs, the reference time has to be shifted. Range: 0-0xFFFFFF
 ** \param[in, out] TimerPtr Pointer to a timer instance defined by the user.
 **
 ** \ServiceID{0x9}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_ShiftTimer1us24bit
(
  P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr,
  uint32 TimeValue
);

/** \brief Synchronizes two timer instances.
 **
 ** \param[in] TimerSrcPtr Pointer to the source timer instance defined by the user.
 ** \param[out] TimerDstPtr Pointer to the destination timer instance defined by the user.
 **
 ** \ServiceID{0xa}
 ** \Reentrancy{Reentrant but not for the same destination timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_SyncTimer1us24bit
(
  P2VAR(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)      TimerDstPtr,
  P2CONST(Tm_PredefTimer1us24bitType, AUTOMATIC, TM_APPL_DATA)    TimerSrcPtr
);

/** \brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] WaitingTimeMin Minimum waiting time in microseconds.
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0xb}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us24bit
(
  uint8 WaitingTimeMin
);

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_24BIT == STD_ON) */

/* !LINKSTO SWS_Tm_00026_2,1, SWS_Tm_00027_2,1 */
#if (TM_ENABLE_PREDEF_TIMER_1US_32BIT == STD_ON)

/** \brief Resets a timer instance (user point of view).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[out] TimerPtr Pointer to a timer instance defined by the user.
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0xc}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer1us32bit
(
  P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr
);

/** \brief Delivers the time difference (current time - reference time).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] TimerPtr Pointer to a timer instance defined by the user.
 ** \param[out] TimeSpanPtr Pointer to time span destination data in RAM
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0xd}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan1us32bit
(
  P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  P2VAR(uint32, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
);

/** \brief Shifts the reference time of the timer instance.
 **
 ** \param[in] TimeValue Time value in μs, the reference time has to be shifted.
 ** \param[in, out] TimerPtr Pointer to a timer instance defined by the user.
 **
 ** \ServiceID{0xe}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_ShiftTimer1us32bit
(
  P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)      TimerPtr,
  uint32 TimeValue
);

/** \brief Synchronizes two timer instances.
 **
 ** \param[in] TimerSrcPtr Pointer to the source timer instance defined by the user.
 ** \param[out] TimerDstPtr Pointer to the destination timer instance defined by the user.
 **
 ** \ServiceID{0xf}
 ** \Reentrancy{Reentrant but not for the same destination timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_SyncTimer1us32bit
(
  P2VAR(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)      TimerDstPtr,
  P2CONST(Tm_PredefTimer1us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerSrcPtr
);

/** \brief Performs busy waiting by polling with a guaranteed minimum waiting time.
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] WaitingTimeMin Minimum waiting time in microseconds.
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x10}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_BusyWait1us32bit
(
  uint8 WaitingTimeMin
);

#endif /* #if (TM_ENABLE_PREDEF_TIMER_1US_32BIT == STD_ON) */

/* !LINKSTO SWS_Tm_00026_3,1 */
#if (TM_ENABLE_PREDEF_TIMER_100US_32BIT == STD_ON)

/** \brief Resets a timer instance (user point of view).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[out] TimerPtr Pointer to a timer instance defined by the user.
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x11}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_ResetTimer100us32bit
(
  P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr
);

/** \brief Delivers the time difference (current time - reference time).
 **
 ** \pre Gpt driver is initialized
 **
 ** \param[in] TimerPtr Pointer to a timer instance defined by the user.
 ** \param[out] TimeSpanPtr Pointer to time span destination data in RAM
 ** \return Standard Return Code
 ** \retval E_OK      The underlying GPT driver service has returned E_OK and no development error
 **                   has been detected
 ** \retval E_NOT_OK  The underlying GPT driver service has returned E_NOT_OK, or a development
 **                   error has been detected
 **
 ** \ServiceID{0x12}
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(Std_ReturnType, TM_CODE) Tm_GetTimeSpan100us32bit
(
  P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)  TimerPtr,
  P2VAR(uint32, AUTOMATIC, TM_APPL_DATA)                          TimeSpanPtr
);

/** \brief Shifts the reference time of the timer instance.
 **
 ** \param[in] TimeValue Time value in unit 100μs, the reference time has to be shifted.
 ** \param[in, out] TimerPtr Pointer to a timer instance defined by the user.
 **
 ** \ServiceID{0x13}
 ** \Reentrancy{Reentrant but not for the same timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_ShiftTimer100us32bit
(
  P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerPtr,
  uint32 TimeValue
);

/** \brief Synchronizes two timer instances.
 **
 ** \param[in] TimerSrcPtr Pointer to the source timer instance defined by the user.
 ** \param[out] TimerDstPtr Pointer to the destination timer instance defined by the user.
 **
 ** \ServiceID{0x14}
 ** \Reentrancy{Reentrant but not for the same destination timer instance}
 ** \Synchronicity{Synchronous}
 **/
extern FUNC(void, TM_CODE) Tm_SyncTimer100us32bit
(
  P2VAR(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)    TimerDstPtr,
  P2CONST(Tm_PredefTimer100us32bitType, AUTOMATIC, TM_APPL_DATA)  TimerSrcPtr
);
#endif /* #if (TM_ENABLE_PREDEF_TIMER_100US_32BIT == STD_ON) */

#define TM_STOP_SEC_CODE
#include <Tm_MemMap.h>

#endif /* ifndef TM_H */

