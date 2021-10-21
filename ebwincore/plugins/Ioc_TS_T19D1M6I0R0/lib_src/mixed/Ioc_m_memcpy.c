/* Ioc_m_memcpy.c
 *
 * This file defines the memory copy function for the IOC.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_m_memcpy.c 2257 2017-09-01 14:35:24Z olme8414 $
*/
/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 11.5 (advisory)
 *  A conversion should not be performed from pointer to void into pointer to
 *  object.
 *
 * Reason:
 *  The given pointer is type converted to a byte pointer, because the pointed
 *  to memory shall be copied to a different location. This operation requires
 *  a pointer to a different type than 'void' and byte access is the least
 *  common denominator. Therefore, the given pointer is converted into a byte
 *  pointer.
 *
 * MISRAC2012-2) Deviated Rule: 10.8 (required)
 *  The value of a composite expression shall not be cast to a different essential
 *  type category or a wider essential type.
 *
 * Reason:
 *  False positive. This isn't a complex expression, it is just a sizeof operator.
 *  There is no simpler way to express this in a portable manner.
 *
 * MISRAC2012-3) Deviated Rule 11.5 (advisory)
 *  A conversion should not be performed from pointer to void to pointer to
 *  object.
 *
 * Reason:
 *  The memory copy operation employs an optimization, which strives to copy
 *  as many whole words as possible instead of copying every single byte.
 *  This requires type conversion chains from word to byte via 'void'.
*/

#include <private/Ioc_memcpy.h>

/* IOC_MemCpy assumes that ioc_address_t the largest atomically accessible data type. */
typedef ioc_address_t ioc_memcpy_word_t;

/* IOC_MemCpy() - copies data from one memory location to another
 *
 * If possible, this function copies data in ioc_memcpy_word_t-sized blocks. Otherwise it copies byte-wise.
 *
 * Parameters:
 *	dest		- reference to destination
 *	src			- reference to source
 *	len			- length of copied area in bytes
 *
 * !LINKSTO IOC.Function.IOC_MemCpy, 1
*/
void IOC_MemCpy(void *dest, const void * const src, ioc_uint32_t len)
{
	/* define pointers for access */
	/* Deviation MISRAC2012-1 <1> */
	ioc_uint8_t *byteDst = dest;
	const ioc_uint8_t * byteSrc = src;
	/* Size of the largest atomically accessible data type */
	/* Deviation MISRAC2012-2 <1> */
	const ioc_address_t wordSize = (ioc_address_t) sizeof(ioc_memcpy_word_t);
	ioc_address_t remainingLen = len;

	/* Word-wise copying is only possible if
	 *  (a) the amount of bytes to be copied exceeds a word and
	 *  (b) source and destination have an identical offset to word-alignment.
	*/
	if ( (len >= wordSize)
		&& ((((ioc_address_t)byteDst) % wordSize) == (((ioc_address_t)byteSrc) % wordSize)) )
	{
		ioc_memcpy_word_t *wordDst;
		const ioc_memcpy_word_t *wordSrc;

		/* copy first area until first word alignment */
		while ((((ioc_address_t)byteSrc) % wordSize) != 0u)
		{
			*byteDst = *byteSrc;
			byteDst++;
			byteSrc++;
			remainingLen--;
		}
		/* copy word-wise as long as there is enough data remaining */
		/* Deviation MISRAC2012-3 <1> */
		wordDst = (ioc_memcpy_word_t *) (void *) byteDst;
		wordSrc = (const ioc_memcpy_word_t *) (const void *) byteSrc;
		while (remainingLen >= wordSize)
		{
			*wordDst = *wordSrc;
			wordDst++;
			wordSrc++;
			remainingLen -= wordSize;
		}
		/* Deviation MISRAC2012-3 <1> */
		byteDst = (ioc_uint8_t *) (void *) wordDst;
		byteSrc = (const ioc_uint8_t *) (const void *) wordSrc;
	}
	/* copy remaining data byte-wise */
	while (remainingLen > 0u)
	{
		*byteDst = *byteSrc;
		byteDst++;
		byteSrc++;
		remainingLen--;
	}
}

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
*/
