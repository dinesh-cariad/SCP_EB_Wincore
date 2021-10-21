/* Os_timeconversion64.h
 *
 * This file defines a collection of time conversion macros (ns-to-ticks and ticks-to-ns)
 * for various frequencies.
 *
 * (c) Elektrobit Automotive GmbH
 *
*/

/* DCG Deviations:
 *
 * DCG-1) Deviated Rule: [OS_C_COMPL_020]
 *  The conditional operator ?: shall not be used.
 *
 * Reason:
 *  The macro is intended to be used for initializing constants, so neither an if ... else construct
 *  nor a function call is possible.
*/
#ifndef OS_TIMECONVERSION64_H
#define OS_TIMECONVERSION64_H

#include <Os_types.h>

/* Commonly-used macros:
 *
 * OS_DivMulDiv() performs the divide-multiply-divide operation used in many ns-to-ticks calculations.
 * OS_MulDivMul() performs the multiply-divide-multiply operation used in many ticks-to-ns calculations.
 * OS_DivMul() divides and then multiplies.
 * OS_SatMulDivMul() performs OS_MulDivMul(), but with a saturation test first.
 * OS_SatDivMul() performs OS_DivMul(), but with a saturation test first.
*/
#define OS_DivMulDiv(x, d1, m, d2)		(((((os_uint64_t)(x))/(d1))*(m))/(d2))
#define OS_MulDivMul(x, m1, d, m2)		(((((os_uint64_t)(x))*(m1))/(d))*(m2))
#define OS_DivMul(x, d, m)			((((os_uint64_t)(x))/(d))*(m))
/* Deviation DCG-1 <+2> */
#define OS_SatMulDivMul(x, m1, d, m2, max)	(((x)>(max)) ? 0xffffffffffffffffuL : OS_MulDivMul((x),(m1),(d),(m2)))
#define OS_SatDivMul(x, d, m, max)		(((x)>(max)) ? 0xffffffffffffffffuL : OS_DivMul((x),(d),(m)))

/*
 * time conversion macros for 2.08333... MHz
*/
#define OS_NsToTicks_2083333(ns)		((ns)/480uL)
#define OS_TicksToNs_2083333(tx)		(((tx) > 38430716820228232uL) ? 0xffffffffffffffffuL : ((tx)*480uL))

/*
 * time conversion macros for 2.5 MHz
*/
#define OS_NsToTicks_2500000(ns)		((ns)/400uL)
#define OS_TicksToNs_2500000(tx)		(((tx) > 46116860184273879uL) ? 0xffffffffffffffffuL : ((tx)*400uL))

/*
 * time conversion macros for 8.333... MHz
*/
#define OS_NsToTicks_8333333(ns)		((ns)/120uL)
#define OS_TicksToNs_8333333(tx)		(((tx) > 153722867280912930uL) ? 0xffffffffffffffffuL : ((tx)*120uL))

/*
 * time conversion macros for 10 MHz
*/
#define OS_NsToTicks_10000000(ns)		((ns)/100uL)
#define OS_TicksToNs_10000000(tx)		(((tx) > 184467440737095516uL) ? 0xffffffffffffffffuL : ((tx)*100uL))

/*
 * time conversion macros for 12 MHz
*/
#define OS_NsToTicks_12000000(ns)		OS_DivMulDiv((ns), 5uL, 3uL, 50uL)
#define OS_TicksToNs_12000000(tx)		OS_SatMulDivMul((tx), 50uL, 3uL, 5uL, 221360928884514619uL)

/*
 * time conversion macros for 33 1/3 MHz
*/
#define OS_NsToTicks_33333333(ns)		((ns)/30uL)
#define OS_TicksToNs_33333333(tx)		(((tx) > 614891469123651720uL) ? 0xffffffffffffffffuL : ((tx)*30uL))

/*
 * time conversion macros for 50 MHz
*/
#define OS_NsToTicks_50000000(ns)		((ns)/20uL)
#define OS_TicksToNs_50000000(tx)		(((tx) > 922337203685477580uL) ? 0xffffffffffffffffuL : ((tx)*20uL))

/*
 * time conversion macros for 100 MHz
*/
#define OS_NsToTicks_100000000(ns)		((ns)/10uL)
#define OS_TicksToNs_100000000(tx)		(((tx) > 1844674407370955161uL) ? 0xffffffffffffffffuL : ((tx)*10uL))

/*
 * time conversion macros for 133 MHz
*/
#define OS_NsToTicks_133000000(ns)		OS_DivMulDiv((ns), 200uL, 133uL, 5uL)
#define OS_TicksToNs_133000000(tx)		OS_SatMulDivMul((tx), 5uL, 133uL, 200uL, 2453416961803370389uL)

/*
 * time conversion macros for 150 MHz
*/
#define OS_NsToTicks_150000000(ns)		OS_DivMulDiv((ns), 4uL, 3uL, 5uL)
#define OS_TicksToNs_150000000(tx)		OS_SatMulDivMul((tx), 5uL, 3uL, 4uL, 2767011611056432742uL)

/*
 * time conversion macros for 500 MHz
*/
#define OS_NsToTicks_500000000(ns)		((ns)/2uL)
#define OS_TicksToNs_500000000(tx)		(((tx) > 9223372036854775807uL) ? 0xffffffffffffffffuL : ((tx)*2uL))

/*
 * time conversion macros for 800 MHz
*/
#define OS_NsToTicks_800000000(ns)		OS_DivMul((ns), 5uL, 4uL)
#define OS_TicksToNs_800000000(tx)      	OS_SatDivMul((tx), 4uL, 5uL, 14757395258967641295uL)

/*
 * time conversion macros for 1000 MHz
*/
#define OS_NsToTicks_1000000000(ns)		(ns)
#define OS_TicksToNs_1000000000(tx)		(tx)

#endif

