/* Os_api_timestamp.h
 *
 * Definitions for a kernel timestamp service API.
 * A timestamp is a counter that can never overflow during the expected up-time of the processor.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_api_timestamp.h 28903 2018-07-04 12:06:04Z swho270744 $
*/
#ifndef OS_API_TIMESTAMP_H
#define OS_API_TIMESTAMP_H

#ifndef OS_ASM

#include <Os_types.h>
#if OS_KERNEL_TYPE == OS_MICROKERNEL
#include <public/Mk_public_api.h>

/* For full descriptions of these macros, see below
 *
 * !LINKSTO Kernel.Feature.TimeStamp, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeGetLo, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeGetHi, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeSub64, 1,
 * !        Kernel.Feature.TimeStamp.API.DiffTime32, 2
 * !doctype SRC
*/
/* Map timestamp API to equivalent microkernel functions. */
#define OS_GetTimeStamp(t)			MK_ReadTime(t)
#if MK_HAS_INT64
/* The 64-bit versions of the macros OS_Time[Get|Set][Lo|Hi]() can be found
 * later in this file.
 */
#else
#define OS_TimeGetLo(t)				((t).timeLo)
#define OS_TimeGetHi(t)				((t).timeHi)
#define OS_TimeSetLo(t,v)	\
	do {					\
		(t).timeLo = (v);	\
	} while (0)
#define OS_TimeSetHi(t,v)	\
	do {					\
		(t).timeHi = (v);	\
	} while (0)
#endif
#define OS_TimeSub64(tr, t1, t2)	MK_DiffTime(tr, t1, t2)
#define OS_DiffTime32(t1, t2)		MK_DiffTime32(t1, t2)

#else
#include <Os_api_arch.h>

#if OS_ARCH_HAS_64BIT
/* For full descriptions of these macros, see below
 *
 * !LINKSTO Kernel.Feature.TimeStamp, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeSub64, 1
 * !doctype SRC
*/
#define OS_TimeSub64(tr, t1, t2)	\
	do {							\
		*(tr) = (*(t1)) - (*(t2));	\
	} while (0)
/* The 64-bit versions of the macros OS_Time[Get|Set][Lo|Hi]() can be found
 * later in this file.
 */

#else	/* !OS_ARCH_HAS_64BIT */

/* OS_TimeGetHi(), OS_TimeGetLo() - return the high and low words (resp.) of a timestamp value
 *
 * !LINKSTO Kernel.Feature.TimeStamp, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeGetLo, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeGetHi, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeSub64, 1
 * !doctype SRC
*/
#define OS_TimeGetLo(t)			((t).tsLo)
#define OS_TimeGetHi(t)			((t).tsHi)
#define OS_TimeSetLo(t,v)	do { (t).tsLo = (v); } while (0)
#define OS_TimeSetHi(t,v)	do { (t).tsHi = (v); } while (0)
#define OS_TimeSub64(tr, t1, t2)					\
	do												\
	{												\
		(tr)->tsHi = (t1)->tsHi - (t2)->tsHi;		\
		if ( (t2)->tsLo > (t1)->tsLo )				\
		{											\
			(tr)->tsHi -= 1;						\
		}											\
		(tr)->tsLo = (t1)->tsLo - (t2)->tsLo;		\
	} while(0)

#endif	/* OS_ARCH_HAS_64BIT */

/* returns the saturated 32-bit time difference. */
extern os_uint32_t OS_DiffTime32(const os_timestamp_t *newTime, const os_timestamp_t *oldTime);

/* Function for the generic timestamp implementation.
 * If an internal timestamp timer is used, this function does nothing.
 *
 * You should not directly call this function. Use OS_GetTimeStamp() instead.
*/
extern void OS_GetTimeStampGeneric(os_timestamp_t *);

/* OS_GetTimeStamp()
 *
 * Return the current value of the timestamp timer.
 * Either this is an internal function defined by the CPU family, or
 * it is a software-extended generic counter.
 *
 * !LINKSTO Kernel.Feature.TimeStamp, 1,
 * !        Kernel.Feature.TimeStamp.API.GetTimeStamp, 1
 * !doctype SRC
 */
#if (OS_HAS_TB_INTERNAL == 1)

/* HW-dependent internal time. The macro OS_TB_FUNC must be set. */
#ifndef OS_TB_FUNC
#error "Timestamp function not defined."
#endif

#define OS_GetTimeStamp(t)	OS_TB_FUNC(t)

#elif (OS_HAS_TB_INTERNAL == 0)

/* Generic implementation */

#define OS_GetTimeStamp(t)	OS_GetTimeStampGeneric(t)

#else
#error "Unsupported value of OS_HAS_TB_INTERNAL."
#endif

#endif
#endif

/* If the Os is part of the MK, OS_ARCH_HAS_64BIT is defined as MK_HAS_INT64.
 * Therefore it is enough to check this macro.
 */
#if (OS_ARCH_HAS_64BIT)
/* For full descriptions of these macros, see above
 *
 * !LINKSTO Kernel.Feature.TimeStamp.API.TimeGetLo, 1,
 * !        Kernel.Feature.TimeStamp.API.TimeGetHi, 1
 * !doctype SRC
*/
#define OS_TimeGetLo(t)				((t) & 0xffffffffU)
#define OS_TimeGetHi(t)				(((t) >> 32U) & 0xffffffffU)
#define OS_TimeSetLo(t,v)														\
	do {																		\
		(t) = ((t) & 0xffffffff00000000UL) | ((os_uint64_t)(v) & 0xffffffffUL);	\
	} while (0)
#define OS_TimeSetHi(t,v)																\
	do {																				\
		(t) = ((t) & 0x00000000ffffffffUL) | (((os_uint64_t)(v) & 0xffffffffUL) << 32U);\
	} while (0)
#endif

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
