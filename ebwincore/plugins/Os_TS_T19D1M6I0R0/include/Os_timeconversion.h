/* Os_timeconversion.h - collection of timer conversion macros.
 *
 * This file defines a collection of time conversion macros (ns-to-ticks and ticks-to-ns)
 * for various frequencies.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Os_timeconversion.h 30527 2019-07-17 10:29:24Z swho270744 $
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

#ifndef OS_TIMECONVERSION_H
#define OS_TIMECONVERSION_H

#include <Os_types.h>

/* Commonly-used macros:
 *
 * OS_DivMulDiv() performs the divide-multiply-divide operation used in many ns-to-ticks calculations
 * OS_MulDivMul() performs the multiply-divide-multiply operation used in many ticks-to-ns calculations
 * OS_DivMul() divides and then multiplies.
 * OS_SatMulDivMul() performs OS_MulDivMul(), but with a saturation test first.
 * OS_SatDivMul() performs OS_DivMul(), but with a saturation test first.
*/
#define OS_DivMulDiv(x, d1, m, d2)			(((((os_uint32_t)(x))/(d1))*(m))/(d2))
#define OS_MulDivMul(x, m1, d, m2)			(((((os_uint32_t)(x))*(m1))/(d))*(m2))
#define OS_DivMul(x, d, m)					((((os_uint32_t)(x))/(d))*(m))
/* Deviation DCG-1 <+2> */
#define OS_SatMulDivMul(x, m1, d, m2, max)	(((x)>(max)) ? 0xffffffffu : OS_MulDivMul((x),(m1),(d),(m2)))
#define OS_SatDivMul(x, d, m, max)			(((x)>(max)) ? 0xffffffffu : OS_DivMul((x),(d),(m)))


/* Clock frequency 10 kHz
*/
#define OS_NsToTicks_10000(ns)		((ns)/100000u)
#define OS_TicksToNs_10000(tx)		(((tx) > 42949u) ? 0xffffffffu: ((tx) * 100000u))

/* Clock frequency 16 KHz
*/
#define OS_NsToTicks_16000(ns)		((ns)/62500u)
#define OS_TicksToNs_16000(tx)		(((tx) > 68719u) ? 0xffffffffu : ((tx)*62500u))

/* Clock frequency 1,001675 MHz
*/
#define OS_NsToTicks_1001675(ns)	OS_DivMulDiv((ns), 50000u, 40067u, 800u)
#define OS_TicksToNs_1001675(tx)	OS_SatMulDivMul((tx), 800u, 40067u, 50000u, 4302161u)

/* Clock frequency 2 Mhz
*/
#define OS_NsToTicks_2000000(ns)	((ns)/500u)
#define OS_TicksToNs_2000000(tx)	(((tx) > 8589934u) ? 0xffffffffu : ((tx)*500u))

/* Clock frequency 2.08 Mhz
*/
#define OS_NsToTicks_2080000(ns)	OS_DivMulDiv((ns), 25u, 13u, 250u)
#define OS_TicksToNs_2080000(tx)	OS_SatMulDivMul((tx), 250u, 13u, 25u, 8933531u)

/* Clock frequency 3 MHz
 *
 * Factor is 3000000/1000000000
 *         = 3/1000
*/
#define OS_NsToTicks_3000000(ns)	OS_DivMulDiv((ns), 4u, 3u, 250u)
#define OS_TicksToNs_3000000(tx)	OS_SatMulDivMul((tx), 250u, 3u, 4u, 12884901u)

/* Clock frequency 3.25 MHz
 *
 * Factor is 3250000/1000000000
 *         = 325/100000
 *         = 13/4000
*/
#define OS_NsToTicks_3250000(ns)	OS_DivMulDiv((ns), 16u, 13u, 250u)
#define OS_TicksToNs_3250000(tx)	OS_SatMulDivMul((tx), 250u, 13u, 16u, 13958643u)

/* Clock frequency 3.5 MHz
 *
 * Factor is 3500000/1000000000
 *         = 35/10000
 *         = 7/2000
*/
#define OS_NsToTicks_3500000(ns)	OS_DivMulDiv((ns), 8u, 7u, 250u)
#define OS_TicksToNs_3500000(tx)	OS_SatMulDivMul((tx), 250u, 7u, 8u, 15032385u)

/* Clock frequency 4 MHz
 *
 * Factor is 4000000/1000000000
 *         = 4/1000
 *         = 1/250
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_4000000(ns)	((ns)/250u)
#define OS_TicksToNs_4000000(tx)	(((tx) > 17179869u) ? 0xffffffffu : ((tx)*250u))

/* Clock frequency 5 MHz
 *
 * Factor is 5000000/1000000000
 *         = 5/1000
 *         = 1/200
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_5000000(ns)	((ns)/200u)
#define OS_TicksToNs_5000000(tx)	(((tx) > 21474836u) ? 0xffffffffu : ((tx)*200u))

/* Clock frequency 6.25 MHz
 *
 * Factor is 6250000/1000000000
 *         = 25/4000
 *         = 1/160
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_6250000(ns)	((ns)/160u)
#define OS_TicksToNs_6250000(tx)	(((tx) > 26843545u) ? 0xffffffffu : ((tx)*160u))

/* Clock frequency 8 MHz
 *
 * Factor is 8000000/1000000000
 *         = 8/1000
 *         = 1/125
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_8000000(ns)	((ns)/125u)
#define OS_TicksToNs_8000000(tx)	(((tx) > 34359738u) ? 0xffffffffu : ((tx)*125u))

/* Clock frequency 10 MHz
 *
 * Factor is 10000000/1000000000
 *         = 10/1000
 *         = 1/100
 * No mul-div-mul needed.
 * max = floor(PA_COUNTER_MAXALLOWEDVALUE / 100) = 42949672
*/
#define OS_NsToTicks_10000000(ns)	((ns)/100u)
#define OS_TicksToNs_10000000(tx)	(((tx) > 42949672u) ? 0xffffffffu : ((tx)*100u))

/* Clock frequency 12.5 MHz
 * Factor is 12500000/1000000000
 *         = 125/10000
 *         = 1/80
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_12500000(ns)	((ns)/80u)
#define OS_TicksToNs_12500000(tx)	(((tx) > 53687091u) ? 0xffffffffu : ((tx)*80u))

/* Clock frequency 15 MHz
 *
 * Factor is 15000000/1000000000
 *         = 15/1000
 *         = 3/200
 * 200 = 4*50, and 50>3, so 50 is the first division factor.
*/
#define OS_NsToTicks_15000000(ns)	OS_DivMulDiv((ns), 4u, 3u, 50u)
#define OS_TicksToNs_15000000(tx)	OS_SatMulDivMul((tx), 50u, 3u, 4u, 64424509u)

/* Clock frequency 16 MHz
 *
 * Factor is 16000000/1000000000
 *         = 16/1000
 *         = 2/125
 * 125 = 5*25 and 5>2, so 5 is the first division factor
*/
#define OS_NsToTicks_16000000(ns)	OS_DivMulDiv((ns), 5u, 2u, 25u)
#define OS_TicksToNs_16000000(tx)	OS_SatMulDivMul((tx), 25u, 2u, 5u, 68719476u)

/* Clock frequency 20 MHz
 *
 * Factor is 20000000/1000000000
 *         = 20/1000
 *         = 1/50
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_20000000(ns)	((ns)/50u)
#define OS_TicksToNs_20000000(tx)	( ((tx) > 85899345u) ? 0xffffffffu : ((tx)*50u) )

/* Clock frequency 24 Mhz
 *
 * Factor is 24000000/1000000000
 *         = 3/125
 * 125 = 5 * 25 with 5 > 3, so 5 is the first division factor.
*/
#define OS_NsToTicks_24000000(ns)	OS_DivMulDiv((ns), 5u, 3u, 25u)
#define OS_TicksToNs_24000000(tx)	OS_SatMulDivMul((tx), 25u, 3u, 5u, 103079215u)

/* Clock frequency 24.55 Mhz
 *
 * Factor is 24550000/1000000000
 *         = 491/20000
 * 20000 = 500 * 40 with 500 > 491, so 500 is the first division factor.
*/
#define OS_NsToTicks_24550000(ns)	OS_DivMulDiv((ns), 500u, 491u, 40u)
#define OS_TicksToNs_24550000(tx)	OS_SatMulDivMul((tx), 40u, 491u, 500u, 105441447u)

/* Clock frequency 25 MHz
 *
 * Factor is 25000000/1000000000
 *         = 25/1000
 *         = 1/40
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_25000000(ns)	((ns)/40u)
#define OS_TicksToNs_25000000(tx)	(((tx) > 107374182u) ? 0xffffffffu : ((tx)*40u))

/* Clock frequency 26 MHz
 *
 */
#define OS_NsToTicks_26000000(ns)	OS_DivMulDiv((ns), 20u, 13u, 25u)
#define OS_TicksToNs_26000000(tx)	OS_SatMulDivMul((tx), 25u, 13u, 20u, 111669149u)

/* Clock frequency 32 MHz
 *
 * Factor is 32000000/1000000000
 *         = 32/1000
 *         = 4/125
 * 125 = 5*25 and 5>4, so 5 is the first division factor
*/
#define OS_NsToTicks_32000000(ns)	OS_DivMulDiv((ns), 5u, 4u, 25u)
#define OS_TicksToNs_32000000(tx)	OS_SatMulDivMul((tx), 25u, 4u, 5u, 137438953u)

/* Clock frequency 33 MHz
 *
 * Factor is 33000000/1000000000
 *         = 33/1000
 * 1000 = 40*25 and 40>33, so 40 is the first division factor
*/
#define OS_NsToTicks_33000000(ns)	OS_DivMulDiv((ns), 40u, 33u, 25u)
#define OS_TicksToNs_33000000(tx)	OS_SatMulDivMul((tx), 25u, 33u, 40u, 141733920u)

/* Clock frequency 33,3 MHz
 *
 * Factor is 33300000/1000000000
 *         = 333/10000
*/
#define OS_NsToTicks_33300000(ns)	OS_DivMulDiv((ns), 400u, 333u, 25u)
#define OS_TicksToNs_33300000(tx)	OS_SatMulDivMul((tx), 25u, 333u, 400u, 143022410u)

/* Clock frequency 37.5 MHz
 *
 * Factor is 37500000/1000000000
 *         = 375/10000
 *         = 3/80
 * 80 = 4 * 20 and 4>3, so 4 is the first division factor
*/
#define OS_NsToTicks_37500000(ns)	OS_DivMulDiv((ns), 4u, 3u, 20u)
#define OS_TicksToNs_37500000(tx)	OS_SatMulDivMul((tx), 20u, 3u, 4u, 161061273u)

/* Clock frequency 40 MHz
 *
 * Factor is 40000000/1000000000
 *         = 40/1000
 *         = 1/25
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_40000000(ns)	((ns)/25u)
#define OS_TicksToNs_40000000(tx)	( ((tx) > 171798691u) ? 0xffffffffu : ((tx)*25u) )

/* Clock frequency 45 MHz
 *
 * Factor is 45000000/1000000000
 *         = 45/1000
 *         = 9/200
 * 200 = 20*10, and 10>9, so 10 is the first division factor.
*/
#define OS_NsToTicks_45000000(ns)	OS_DivMulDiv((ns), 10u, 9u, 20u)
#define OS_TicksToNs_45000000(tx)	OS_SatMulDivMul((tx), 20u, 9u, 10u, 193273528u)

/* Clock frequency 48 MHz
 *
 * Factor is 48000000/1000000000
 *         = 48/1000
 *         = 6/125
 * 125 = 25*5 and 25>6, so 25 is the first division factor
*/
#define OS_NsToTicks_48000000(ns)	OS_DivMulDiv((ns), 25u, 6u, 5u)
#define OS_TicksToNs_48000000(tx)	OS_SatMulDivMul((tx), 5u, 6u, 25u, 206158430u)

/* Clock frequency 50 MHz
 *
 * Factor is 50000000/1000000000
 *         = 5/100
 *         = 1/20
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_50000000(ns)	((ns)/20u)
#define OS_TicksToNs_50000000(tx)	( ((tx) > 214748364u) ? 0xffffffffu : ((tx)*20u) )

/* Clock frequency 55 MHz
 *
 * Factor is 55000000/1000000000
 *         = 55/1000
 *         = 11/200
 * 200 = 20*10, and 20>11, so 20 is the first division factor
*/
#define OS_NsToTicks_55000000(ns)	OS_DivMulDiv((ns), 20u, 11u, 10u)
#define OS_TicksToNs_55000000(tx)	OS_SatMulDivMul((tx), 10u, 11u, 20u, 236223201u)

/* Clock frequency 56 MHz
 *
 * Factor is 56000000/1000000000
 *         = 56/1000
 *         = 7/125
 * 125 = 25*5 and 25>7, so 25 is the first division factor
*/
#define OS_NsToTicks_56000000(ns)	OS_DivMulDiv((ns), 25u, 7u, 5u)
#define OS_TicksToNs_56000000(tx)	OS_SatMulDivMul((tx), 5u, 7u, 25u, 240518168u)

/* Clock frequency 60 MHz
 *
 * Factor is 60000000/1000000000
 *         = 60/1000
 *         = 3/50
 * 50 = 25*2, and 25>3, so 25 is the first division factor
*/
#define OS_NsToTicks_60000000(ns)	OS_DivMulDiv((ns), 5u, 3u, 10u)
#define OS_TicksToNs_60000000(tx)	OS_SatMulDivMul((tx), 10u, 3u, 5u, 257698037u)

/* Clock frequency 62.5 MHz
 *
 * Factor is 125000000/2000000000
 *         = 125/2000
 *         = 5/80
 * 80 = 8*10, and 8>5, so 8 is the first division factor
*/
#define OS_NsToTicks_62500000(ns)	OS_DivMulDiv((ns), 8u, 5u, 10u)
#define OS_TicksToNs_62500000(tx)	OS_SatMulDivMul((tx), 10u, 5u, 8u, 268435455u)

/* Clock frequency 64 MHz
 *
 * Factor is 64000000/1000000000
 *         = 64/1000
 *         = 8/125
 * 125 = 25*5 and 25>8, so 25 is the first division factor
*/
#define OS_NsToTicks_64000000(ns)	OS_DivMulDiv((ns), 25u, 8u, 5u)
#define OS_TicksToNs_64000000(tx)	OS_SatMulDivMul((tx), 5u, 8u, 25u, 274877906u)

/* Clock frequency 70 MHz
 *
 * Factor is 70000000/1000000000
 *         = 7/100
 * 100 = 10*10, so there 10 is the first and last divisor.
*/
#define OS_NsToTicks_70000000(ns)	OS_DivMulDiv((ns), 10u, 7u, 10u)
#define OS_TicksToNs_70000000(tx)	OS_SatMulDivMul((tx), 10u, 7u, 10u, 300647710u)

/* Clock frequency 75 MHz
 *
 * Factor is 75000000/1000000000
 *         = 75/1000
 *         = 3/40
 * 40 = 4*10, and 4>3, so 4 is the first division factor
*/
#define OS_NsToTicks_75000000(ns)	OS_DivMulDiv((ns), 4u, 3u, 10u)
#define OS_TicksToNs_75000000(tx)	OS_SatMulDivMul((tx), 10u, 3u, 4u, 322122547u)

/* Clock frequency 80 MHz
 *
 * Factor is 80000000/1000000000
 *         = 80/1000
 *         = 2/25
 * 25 = 5*5 and 5>2, so 5 is the first division factor
*/
#define OS_NsToTicks_80000000(ns)	OS_DivMulDiv((ns), 5u, 2u, 5u)
#define OS_TicksToNs_80000000(tx)	OS_SatMulDivMul((tx), 5u, 2u, 5u, 343597383u)

/* Clock frequency 90 MHz
 *
 * Factor is 90000000/1000000000
 *         = 90/1000
 *         = 9/100
 * 100 = 10*10 and 10>9, so 10 is the first division factor
*/
#define OS_NsToTicks_90000000(ns)	OS_DivMulDiv((ns), 10u, 9u, 10u)
#define OS_TicksToNs_90000000(tx)	OS_SatMulDivMul((tx), 10u, 9u, 10u, 386547056u)

/* Clock frequency 96 MHz
 *
 * Factor is 96000000/1000000000
 *         = 96/1000
 *         = 12/125
 *
 * 125 = 25 * 5 and 25>12, so 25 is the first division factor.
*/
#define OS_NsToTicks_96000000(ns)	OS_DivMulDiv((ns), 25u, 12u, 5u)
#define OS_TicksToNs_96000000(tx)	OS_SatMulDivMul((tx), 5u, 12u, 25u, 412316860u)

/* Clock frequency 100 MHz
 *
 * Factor is 100000000/1000000000
 *         = 1/10
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_100000000(ns)	((ns)/10u)
#define OS_TicksToNs_100000000(tx)	( ((tx) > 429496729u) ? 0xffffffffu : ((tx)*10u) )

/* Clock frequency 110 MHz
 *
 * Factor is 110000000/1000000000
 *         = 11/100
 * 100 = 20*5, and 20>11, so 20 is the first division factor
 *
 * The limit is calculated by hand because the rounding error in the ns-to-ticks macro gives an incorrect limit.
*/
#define OS_NsToTicks_110000000(ns)	OS_DivMulDiv((ns), 20u, 11u, 5u)
#define OS_TicksToNs_110000000(tx)	OS_SatMulDivMul((tx), 5u, 11u, 20u, 472446402u)

/* Clock frequency 120 MHz
 *
 * Factor is 120000000/1000000000
 *         = 12/100
 *         = 3/25
 * 25 = 5*5, and 5>3, so 5 is the first division factor.
*/
#define OS_NsToTicks_120000000(ns)	OS_DivMulDiv((ns), 5u, 3u, 5u)
#define OS_TicksToNs_120000000(tx)	OS_SatMulDivMul((tx), 5u, 3u, 5u, 515396075u)

/* Clock frequency 132 MHz
 *
 * Factor is 132000000/1000000000
 *         = 132/1000 = 2*2*( 3*11/ 2*5*5*5)
 *         = 33/250
 * 250 = 50*5, and 50>33, so 50 is the first divisor and 5 the second.
*/
#define OS_NsToTicks_132000000(ns)	OS_DivMulDiv((ns), 50u, 33u, 5u)
#define OS_TicksToNs_132000000(tx)	OS_SatMulDivMul((tx), 5u, 33u, 50u, 566935682u)

/* Clock frequency 133 MHz
*/
#define OS_NsToTicks_133000000(ns)	OS_DivMulDiv((ns), 200u, 133u, 5u)
#define OS_TicksToNs_133000000(tx)	OS_SatMulDivMul((tx), 5u, 133u, 200u, 571230650u)

/* Clock frequency 140 MHz
 *
 * Factor is 140000000/1000000000
 *         = 14/100
 *         = 7/50
 * 50 = 10*5, and 10>7, so 10 is the first division factor.
*/
#define OS_NsToTicks_140000000(ns)	OS_DivMulDiv((ns), 10u, 7u, 5u)
#define OS_TicksToNs_140000000(tx)	OS_SatMulDivMul((tx), 5u, 7u, 10u, 601295421u)

/* Clock frequency 125 MHz
 *
 * Factor is 125_000_000/1_000_000_000
 *         = 125/1000
 *         = 1/8
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_125000000(ns)	((ns)/8u)
#define OS_TicksToNs_125000000(tx)	(((tx) > 536870911u) ? 0xffffffffu : ((tx)*8u))

/* Clock frequency 150 MHz
 *
 * Factor is 150000000/1000000000
 *         = 15/100
 *         = 3/20
 * 20 = 4*5, and 5>3, so 5 is the first division factor.
*/
#define OS_NsToTicks_150000000(ns)	OS_DivMulDiv((ns), 4u, 3u, 5u)
#define OS_TicksToNs_150000000(tx)	OS_SatMulDivMul((tx), 5u, 3u, 4u, 644245094u)

/* Clock frequency 160 MHz
 *
 * Factor is 160000000/1000000000
 *         = 16/100
 *         = 4/25
 * 25 = 5*5, and 5>4, so 5 is the first division factor.
*/
#define OS_NsToTicks_160000000(ns)	OS_DivMulDiv((ns), 5u, 4u, 5u)
#define OS_TicksToNs_160000000(tx)	OS_SatMulDivMul((tx), 5u, 4u, 5u, 687194767u)

/* Clock frequency 180 MHz
 *
 * Factor is 180000000/1000000000
 *         = 18/100
 *         = 9/50
 * 50 = 5*10 and 10>9, so 10 is the first division factor
*/
#define OS_NsToTicks_180000000(ns)	OS_DivMulDiv((ns), 10u, 9u, 5u)
#define OS_TicksToNs_180000000(tx)	OS_SatMulDivMul((tx), 5u, 9u, 10u, 773094113u)

/* Clock frequency 200 MHz
 *
 * Factor is 200000000/1000000000
 *         = 1/5
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_200000000(ns)	((ns)/5u)
#define OS_TicksToNs_200000000(tx)	(((tx) > 858993459u) ? 0xffffffffu : ((tx)*5u))

/* Clock frequency 240 MHz
 *
 * Factor is 240000000/1000000000
 *         = 24/100
 *         = 6/25
 * 25 = 5*5, but 5 < 6.
*/
#define OS_NsToTicks_240000000(ns)	OS_DivMul((ns), 25u, 6u)
#define OS_TicksToNs_240000000(tx)	OS_SatDivMul((tx), 6u, 25u, 1030792150u)


/* Clock frequency 250 MHz
 *
 * Factor is 250000000/1000000000
 *         = 1/4
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_250000000(ns)	((ns)/4u)
#define OS_TicksToNs_250000000(tx)	(((tx) > 1073741823u) ? 0xffffffffu : ((tx)*4u))

/* Clock frequency 264 MHz
 *
 * Factor is 264000000/1000000000
 *         = 264/1000 = 2*2*2*( 3*11/ 5*5*5)
 *         = 33/125
 * 125 = 25*5, and 25<33, so 125 is the first divisor and 1 the second.
*/
#define OS_NsToTicks_264000000(ns)	OS_DivMulDiv((ns), 125u, 33u, 1u)
#define OS_TicksToNs_264000000(tx)	OS_SatMulDivMul((tx), 1u, 33u, 125u, 1133871365u)

/* Clock frequency 300 MHz
 *
 * Factor is 300000000/1000000000
 *         = 3/10
 * 10 = 5*2 and 5>3, so 5 is the first division factor
*/
#define OS_NsToTicks_300000000(ns)	OS_DivMulDiv((ns), 5u, 3u, 2u)
#define OS_TicksToNs_300000000(tx)	OS_SatMulDivMul((tx), 2u, 3u, 5u, 1288490188u)

/* Clock frequency 330 MHz
 *
 * Factor is 330000000/1000000000
 *         = 33/100
 * 100 = 50*2 and 50>33, so 50 is the first division factor
*/
#define OS_NsToTicks_330000000(ns)	OS_DivMulDiv((ns), 50u, 33u, 2u)
#define OS_TicksToNs_330000000(tx)	OS_SatMulDivMul((tx), 2u, 33u, 50u, 1417339207u)

/* Clock frequency 400 MHz
 *
 * Factor is 400000000/1000000000
 *         = 2/5
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_400000000(ns)	(((ns)/5u)*2u)
#define OS_TicksToNs_400000000(tx)	(((tx) > 1717986918u) ? 0xffffffffu : (((tx)/2u)*5u))

/* Clock frequency 800 MHz
 *
 * Factor is 800000000/1000000000
 *         = 4/5
 * No mul-div-mul needed.
*/
#define OS_NsToTicks_800000000(ns)	(((ns)/5u)*4u)
#define OS_TicksToNs_800000000(tx)	(((tx) > 3435973836u) ? 0xffffffffu : (((tx)/4u)*5u))

#endif
/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
