/* kern-countleadingzeros.c
 *
 * This file contains the OS_CountLeadingZeros function.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: kern-countleadingzeros.c 23286 2015-12-22 12:05:22Z tojo2507 $
*/

#include <Os_kernel.h>

#include <memmap/Os_mm_code_begin.h>

#if OS_SIZEOF_CLZWORD==8

#define OS_MASK32	0xffffffff00000000u
#define OS_MASK16	0xffff0000ffff0000u
#define OS_MASK8	0xff00ff00ff00ff00u
#define OS_MASK4	0xf0f0f0f0f0f0f0f0u
#define OS_MASK2	0xccccccccccccccccu
#define OS_MASK1	0xaaaaaaaaaaaaaaaau

#elif OS_SIZEOF_CLZWORD==4

#define OS_MASK16	0xffff0000u
#define OS_MASK8	0xff00ff00u
#define OS_MASK4	0xf0f0f0f0u
#define OS_MASK2	0xccccccccu
#define OS_MASK1	0xaaaaaaaau

#elif OS_SIZEOF_CLZWORD==2

#define OS_MASK8	0xff00u
#define OS_MASK4	0xf0f0u
#define OS_MASK2	0xccccu
#define OS_MASK1	0xaaaau

#else
#error "OS_SIZEOF_CLZWORD is not defined. Check your architecture header files!"
#endif

/* OS_CountLeadingZeros
 *
 * This function returns the number of leading zeros in the binary representation of the input parameter.
 *
 * How it works:
 * =============
 *
 * First of all, if the word is zero we return NO-OF-BITS-IN-WORD.
 * The remainder of the function is a binary search for the most significant bit.
 * We start with a count of 0.
 * At each stage we are examining the upper n/2 bits of the n bits that remain to be examined.
 * If the upper n/2 bits are all zero (determined by an AND mask) we add n/2 to the count.
 * If one or more of the upper n/2 bits is non-zero, we discard the lower n/2 bits by retaining the
 * result of the AND mask.
 * The masks used in each successive stage contain alternately n/2 1s followed by n/2 0s. At each stage
 * we have discarded either the lower n/2 bits (explicitly) or the upper n/2 bits (implicitly, by determining
 * that they're all zero), so we "focus in" on the MSB.
 *
 * This masking is done instread of the more obvious mask and shift on efficiency grounds. On some processors
 * a multi-bit shift takes several cycles. Furthermore, the result of the AND should already be present in a register,
 * so no further operation is needed. An optimising compiler ought to be able to make the code very small,
 * but in some cases it might be better to implement this function in assembly language. Some processors
 * even have count-leading-zeros available as an instruction, which should obviously be used if possible.
 *
 * It would be possible to place the stages in a loop, but an optimising compiler might well unroll the loop,
 * and in any case the function needs to be as fast as possible, so leaving the loop unrolled in the source
 * code would force that even when optimising for size, since this function needs to be as fast as possible.
*/
os_int_t OS_CountLeadingZeros(os_clzword_t word)
{
	os_int_t nlz = 0;

	if ( word == 0 )
	{
		nlz = OS_SIZEOF_CLZWORD * 8;
	}
	else
	{
#if OS_SIZEOF_CLZWORD > 4
		if ( (word & OS_MASK32) == 0 )
		{
			nlz += 32;
		}
		else
		{
			word &= OS_MASK32;
		}
#endif
#if OS_SIZEOF_CLZWORD > 2
		if ( (word & OS_MASK16) == 0 )
		{
			nlz += 16;
		}
		else
		{
			word &= OS_MASK16;
		}
#endif
		if ( (word & OS_MASK8) == 0 )
		{
			nlz += 8;
		}
		else
		{
			word &= OS_MASK8;
		}

		if ( (word & OS_MASK4) == 0 )
		{
			nlz += 4;
		}
		else
		{
			word &= OS_MASK4;
		}

		if ( (word & OS_MASK2) == 0 )
		{
			nlz += 2;
		}
		else
		{
			word &= OS_MASK2;
		}

		if ( (word & OS_MASK1) == 0 )
		{
			nlz += 1;
		}
	}

	return nlz;
}

#include <memmap/Os_mm_code_end.h>

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
