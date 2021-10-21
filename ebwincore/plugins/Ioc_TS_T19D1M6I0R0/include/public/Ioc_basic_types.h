/* Ioc_basic_types.h
 *
 * This file declares the basic data types used by the IOC.
 * These types are those that are used to define IOC-specific data types.
 * Basic types are always visible to the user.
 * The names of the basic types are chosen in line with POSIX conventions, but prefixed with ioc_.
 * The IOC reuses the standard integer types of the OS.
 *
 * (c) Elektrobit Automotive GmbH
 *
 * $Id: Ioc_basic_types.h 2252 2017-09-01 12:55:57Z olme8414 $
*/
#ifndef IOC_BASIC_TYPES_H
#define IOC_BASIC_TYPES_H

#ifndef IOC_ASM
#include <public/Ioc_libconfig.h>

#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)
#include <public/Mk_basic_types.h>
#include <public/Mk_public_types.h>
#elif (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)
#include <Os_types.h>
#else /* IOC_LCFG_KERNEL_TYPE */
#error "Unknown kernel type."
#endif /* IOC_LCFG_KERNEL_TYPE */


#if (IOC_LCFG_KERNEL_TYPE == IOC_MICROKERNEL)

/* Data type needed for synchronization on certain platforms */
typedef mk_unsigned_t	ioc_unsigned_t;

/* Standard fixed-length signed types
*/
typedef mk_int8_t	ioc_int8_t;
typedef mk_int16_t	ioc_int16_t;
typedef mk_int32_t	ioc_int32_t;

/* Standard fixed-length unsigned types
*/
typedef mk_uint8_t	ioc_uint8_t;
typedef mk_uint16_t	ioc_uint16_t;
typedef mk_uint32_t	ioc_uint32_t;

/* Standard types for memory management
*/
typedef mk_address_t	ioc_address_t;
typedef mk_size_t		ioc_size_t;
typedef mk_boolean_t	ioc_boolean_t;

#elif (IOC_LCFG_KERNEL_TYPE == IOC_AUTOCOREOS)

/* Data type needed for certain compiler intrinsics */
typedef os_unsigned_t	ioc_unsigned_t;

/* Standard fixed-length signed types
*/
typedef os_int8_t	ioc_int8_t;
typedef os_int16_t	ioc_int16_t;
typedef os_int32_t	ioc_int32_t;

/* Standard fixed-length unsigned types
*/
typedef os_uint8_t	ioc_uint8_t;
typedef os_uint16_t	ioc_uint16_t;
typedef os_uint32_t	ioc_uint32_t;

/* Standard types for memory management
*/
typedef os_address_t	ioc_address_t;
typedef os_size_t		ioc_size_t;
typedef os_boolean_t	ioc_boolean_t;

#else /* IOC_LCFG_KERNEL_TYPE */
#error "Unknown kernel type."
#endif /* IOC_LCFG_KERNEL_TYPE */

/* Result type of IOC calls.
*/
typedef ioc_uint8_t		ioc_return_t;

/* Length parameter type used internally by the static API.
 *
 * If ioc_ilenght_t is changed, IOC_WellAlignedILength might need to be adapted.
 * IOC_ILENGTH_MAX is the maximum value, that can be assigned to an ioc_ilength_t variable.
*/
typedef ioc_uint16_t	ioc_ilength_t;
#define IOC_ILENGTH_MAX ((ioc_ilength_t)~(ioc_ilength_t)0u)

/* Length parameter type for variable length AUTOSAR APIs.
 *
 * Note: If varlength_t differs from ilength_t, the IOC implementation
 *  must check, whether the sizes of data elements can be stored in a varlength_t.
*/
typedef ioc_ilength_t	ioc_varlength_t;

/* ioc_extinput_t - Represents data element input, which is passed to extended channels.
 *
 * Note: Don't forget to adapt IOC_HwWellAlignedExtInput, if the alignment requirements of this type change.
*/
typedef struct ioc_extinput_s ioc_extinput_t;
struct ioc_extinput_s
{
	const void		*data;
	ioc_ilength_t	length;
};

/* The index of a channel.
*/
typedef ioc_uint32_t	ioc_channelid_t;

/* Values of ioc_boolean_t.
*/
#define IOC_FALSE		0
#define IOC_TRUE		1

#endif /* IOC_ASM */

/* IOC error-codes */
#define IOC_E_OK				0u
#define IOC_E_NOK				1u
#define IOC_E_LIMIT				130u
#define IOC_E_LOST_DATA			64u
#define IOC_E_NO_DATA			131u

/* These are non-standard return values */
#define IOC_E_SEG_FAULT			136u	/* RTE_E_SEG_FAULT  */
#define IOC_E_ILLEGAL_CALL		3u		/* OS_E_ID */
#define IOC_E_NOFUNC			5u		/* E_OS_NOFUNC */
#define IOC_E_INTRALOCK_FAILED	7u		/* E_OS_STATE */

/* Non-standard return values, invisible to the user */
#define IOC_E_TRYAGAIN			23u

/* The values of the following error-codes aren't specified, yet. */
#define IOC_E_LENGTH			8u		/* E_OS_VALUE */

/* IOC_NULL
 *
 * A null pointer constant
*/
#define IOC_NULL		((void *)0)

#endif /* IOC_BASIC_TYPES_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
