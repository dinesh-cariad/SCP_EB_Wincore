/* Os_timestamp.h
 *
 * Definitions for a kernel timestamp service.
 * A timestamp is a counter that can never overflow during the expected up-time of the processor.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_timestamp.h 27282 2017-08-24 13:57:43Z olme8414 $
*/
#ifndef OS_TIMESTAMP_H
#define OS_TIMESTAMP_H

#ifndef OS_ASM

#include <Os_optimize.h>
#include <Os_types.h>
#include <Os_api_timestamp.h>
#include <Os_kernel_alarm.h>

#if OS_KERNEL_TYPE == OS_MICROKERNEL
/* The microkernel provides its own timestamp. As such it never makes
 * use of the generic timestamp implementation.
 */
#if defined(OS_USEGENERICTIMESTAMP)
#if (OS_USEGENERICTIMESTAMP != 0)
#error "OS_USEGENERICTIMESTAMP incorrectly defined for microkernel."
#endif
#endif

#ifndef OS_USEGENERICTIMESTAMP
#define OS_USEGENERICTIMESTAMP	0
#endif

#define OS_AdvanceTimeStamp(unused)	do { /* nothing */ } while (0)

#else	/* OS_KERNEL_TYPE != OS_MICROKERNEL */


#if OS_ARCH_HAS_64BIT

/* For full descriptions of these macros, see below
*/
#define OS_TimeZero(t)				do { \
										*(t) = (os_timestamp_t)0; \
									} while (0)

#define OS_TimeIsZero(t)			((t) == (os_timestamp_t)0)

#define OS_TimeSub32(t1, t2)		( (os_uint32_t)(t1) - (os_uint32_t)(t2) )

#define OS_TimeSubTickT(t1, t2)		( (os_tick_t)(t1) - (os_tick_t)(t2) )

#define OS_TimeLt(t1, t2)			( (t1) < (t2) )

#define OS_TimeGe(t1, t2)			( (t1) >= (t2) )

#define OS_TimeCopy(t1, t2)			do { \
										*(t1) = (t2); \
									} while (0)

#define OS_TimeAdd32(tr, t1, t2)	do { \
										*(tr) = (t1) + (os_timestamp_t)(t2); \
									} while (0)

#else	/* !OS_ARCH_HAS_64BIT */

/* In all the manipulation macros, "out" parameters are pointers. This is the standard C
 * convention and means that the macros can be replaced with functions if necessary.
*/

/* OS_TimeZero() - sets the "out" timestamp to zero
*/
#define OS_TimeZero(t)	\
	do {				\
		(t)->tsHi = 0u;	\
		(t)->tsLo = 0u;	\
	} while (0)

/* OS_TimeIsZero() - evaluates to true if the timestamp is zero
*/
#define OS_TimeIsZero(t)			( ((t).tsHi ==  0u) && ((t).tsLo == 0u) )

/* OS_TimeSub32() - evaluates to the difference between 2 timestamps truncated to 32-bits
*/
#define OS_TimeSub32(t1, t2)		((t1).tsLo - (t2).tsLo)

/* OS_TimeSubTickT() - evaluates to the difference between 2 timestamps truncated to os_tick_t
*/
#define OS_TimeSubTickT(t1, t2)		((os_tick_t)((t1).tsLo - (t2).tsLo))

/* OS_TimeLt() - evaluates to true if the first timestamp is less than the second
*/
#define OS_TimeLt(t1, t2)	(   ((t1).tsHi < (t2).tsHi) \
							 || ( ((t1).tsHi == (t2).tsHi) && ((t1).tsLo < (t2).tsLo) ) \
							)

/* OS_TimeGe() - evaluates to true if the first timestamp is greater than or equal to the second
*/
#define OS_TimeGe(t1, t2)	(   ((t1).tsHi > (t2).tsHi) \
							 || ( ((t1).tsHi == (t2).tsHi) && ((t1).tsLo >= (t2).tsLo) ) \
							)

/* OS_TimeCopy() - copies the "in" timestamp to the "out" timestamp
*/
#define OS_TimeCopy(t1, t2) \
	do {											\
		(t1)->tsLo = (t2).tsLo;						\
		(t1)->tsHi = (t2).tsHi;						\
	} while(0)

/* OS_TimeAdd32() - add a 32-bit increment onto a timestamp value and place the result in another timestamp value.
 *
 * Note: the upper word is calculated first so that the output can be the same as the input.
*/
#define OS_TimeAdd32(tr, t1, t2) \
	do {											\
		(tr)->tsHi = (t1).tsHi;						\
		if ( (t1).tsLo > (0xffffffffU - (t2)) )		\
		{											\
			(tr)->tsHi++;							\
		}											\
		(tr)->tsLo = (t1).tsLo + (t2);				\
	} while(0)

#endif	/* OS_ARCH_HAS_64BIT */

#ifndef OS_USEGENERICTIMESTAMP
#define OS_USEGENERICTIMESTAMP	1
#define OS_USE_GLOBAL_GENERICTIMESTAMP 1
#endif

#if OS_USEGENERICTIMESTAMP
/* Definitions, variables and functions for a generic timestamp feature
 * that's driven from a selected hardware timer
*/
#ifndef OS_AdvanceTimeStamp
#define OS_AdvanceTimeStamp(t)	OS_AdvanceTimeStampGeneric(t)
#endif
#ifndef OS_INITTIMESTAMP
#define OS_INITTIMESTAMP		(&OS_StartTimeStampTimerGeneric)
#endif

extern os_hwt_t const * const OS_timeStampTimer;
extern os_timervalue_t OS_lastTimeStampTime;
extern os_timestamp_t OS_timeStampValue;

void OS_AdvanceTimeStampGeneric(os_timervalue_t);
void OS_StartTimeStampTimerGeneric(void);

#endif /* OS_USEGENERICTIMESTAMP */
#endif	/* OS_KERNEL_TYPE == OS_MICROKERNEL */
#endif	/* OS_ASM */
#endif	/* OS_TIMESTAMP_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
