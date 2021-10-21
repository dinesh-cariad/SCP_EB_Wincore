/* Os_types_WINLIN.h
 *
 * Data types for WINLIN.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_types_WINLIN.h 31292 2020-01-29 10:17:25Z waab271441 $
*/

#ifndef OS_TYPES_WINLIN_H
#define OS_TYPES_WINLIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <Os_tool.h>

/** \def OS_ENDIAN
 * \brief Defines the endianess.
 */
#define OS_ENDIAN	OS_LITTLEENDIAN

/** \typedef OS_ARCH_HAS_64BIT
 *  \brief Switch to control 64-bit support.
 *
 * Setting this switch to 1 enables 64-bit support and enlarges
 * most data types to 64-bit, e.g. pointers.
 * Set this switch to 0 to target 32-bit architectures or versions
 * of Windows.
 *
 * This switch is usually controlled from the Makefiles, e.g. WINDOWS_WIN32X86_vc_cfg.mak
 * or LINUX_LXTHREADSX86_gcc_cfg.mak, and located on the command-line to affect all files equally.
 */
#ifndef OS_ARCH_HAS_64BIT
#define OS_ARCH_HAS_64BIT	0
#endif	/* !OS_ARCH_HAS_64BIT */

/** \def OS_SIZEOF_PTR
 * \brief Sizes for computing struct offsets for assembler.
 */
#if OS_ARCH_HAS_64BIT
#define OS_SIZEOF_PTR	OS_U(8)
#else
#define OS_SIZEOF_PTR	OS_U(4)
#endif
#define OS_SIZEOF_FPTR	OS_SIZEOF_PTR

#define OS_ARCH_HAS_CHAR_T 1
typedef char			os_char_t;
typedef unsigned char	os_uchar_t;

/** \def OS_STACKLEN_T
 * \brief Data type for type of stack length vars/consts.
 *
 * Leave undefined if default (os_uint16_t) is OK
 */
#if OS_ARCH_HAS_64BIT
#define OS_STACKLEN_T			os_uint64_t
#define OS_SIZEOF_STACKLEN		8
#else
#define OS_STACKLEN_T			os_uint32_t
#define OS_SIZEOF_STACKLEN		4
#endif

#ifndef OS_ASM
/** \name Fixed-length types */
/** @{ */

typedef unsigned char		os_uint8_t;
typedef signed char			os_int8_t;
typedef unsigned short		os_uint16_t;
typedef signed short		os_int16_t;
typedef unsigned int		os_uint32_t;
typedef signed int			os_int32_t;

#if OS_ARCH_HAS_64BIT
typedef long long			os_int64_t;
typedef unsigned long long	os_uint64_t;
#endif

/* Types for address/size of memory object */
#if OS_ARCH_HAS_64BIT
typedef os_uint64_t			os_address_t;
typedef os_int64_t			os_ptrdiff_t;
typedef os_uint64_t			os_size_t;
#else
typedef os_uint32_t			os_address_t;
typedef os_int32_t			os_ptrdiff_t;
typedef os_uint32_t			os_size_t;
#endif

/* Type for a stack element. */
#if OS_ARCH_HAS_64BIT
typedef os_uint64_t			os_stackelement_t;
#else
typedef os_uint32_t			os_stackelement_t;
#endif

/** \typedef os_paramtype_t
 * \brief Type for a service parameter.
 *
 * An integer type big enough to hold any OS service parameter.
 * Used for error handling and system calls.
 */
#if OS_ARCH_HAS_64BIT
typedef os_uint64_t			os_paramtype_t;
#else
typedef os_uint32_t			os_paramtype_t;
#endif

/** \typedef os_winlinresult_t
 * \brief Instances of this type are returned by syscall handlers.
 *
 * Usually, only 8 bits are returned, but GetISRID() returns 16 bits (os_isrid_t).
 * To avoid truncation or undefined bits in the result, this special type plus
 * the function OS_WINLINGetResultFromSyscallBuffer() are necessary.
 * The type os_result_t can't be used, because it's too narrow.
 */
typedef os_uint16_t			os_winlinresult_t;

/** \typedef os_timervalue_t
 * \brief The type to represent timer values.
 */
typedef os_uint32_t os_timervalue_t;

/** @} */
#endif /* !OS_ASM */

/** \def OS_SIZEOF_STACKELEMENT
 * \brief Size in bytes of an element on a stack.
 */
#if OS_ARCH_HAS_64BIT
#define OS_SIZEOF_STACKELEMENT	8
#else
#define OS_SIZEOF_STACKELEMENT	4
#endif

/** \name Interrupt level/status types.
 *
 * Used for saving and restoring interrupt status.
 *
 * os_intstatus_t is used internally by the kernel.
 *
 * os_oldlevel_t is the type for the old-level locations used by
 * the API interrupt management functions.
 */
/** @{ */
#ifndef OS_ASM
typedef os_uint16_t os_intstatus_t;
typedef os_uint8_t os_oldlevel_t;
#endif /* !OS_ASM */
#define OS_SIZEOF_OLDLEVEL	1
/** @} */

/* os_clzword_t - type of a word for which a CLZ or similar instruction is defined. */
#if OS_ARCH_HAS_64BIT
#define OS_SIZEOF_CLZWORD	8
#define OS_CLZWORD_NBITS	64
#ifndef OS_ASM
typedef os_uint64_t os_clzword_t;
#endif

#else /* !OS_ARCH_HAS_64BIT */

#define OS_SIZEOF_CLZWORD	4
#define OS_CLZWORD_NBITS	32
#ifndef OS_ASM
typedef os_uint32_t os_clzword_t;
#endif
#endif /* OS_ARCH_HAS_64BIT */

#ifndef OS_ASM
/** \brief The type of a Windows/Linux HANDLE.
 *
 * We cannot include windows.h here, because some definitions there conflict with
 * OSEK/AUTOSAR definitions, so it is necessary to re-define compatible types.
 *
 * \see http://msdn.microsoft.com/en-us/library/aa383751%28VS.85%29.aspx
*/
typedef void* os_winlinHandle_t;

/** \brief The type of a Windows/Linux thread identifier (TID). */
typedef unsigned long os_winlinTid_t;

/** \brief The type of a Windows/Linux DWORD.
 *
 * \see http://msdn.microsoft.com/en-us/library/aa383751%28VS.85%29.aspx
 */
typedef unsigned long os_winlinDword_t;

/** \brief The type of a Windows/Linux boolean.
 *
 * \see http://msdn.microsoft.com/en-us/library/aa383751%28VS.85%29.aspx
 */
typedef int os_winlinBool_t;

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wlong-long"
#endif
#endif

/** \brief Represents a 64-bit timer value. */
typedef long long os_winlinTimerValue_t;

/** \brief Represents a 64-bit unsigned timer value. */
typedef unsigned long long int os_winlinUnsignedTimerValue_t;

#if (OS_TOOL == OS_gnu)
#if OS_GccVersionIsAtLeast(4,6,4)
#pragma GCC diagnostic pop
#endif
#endif

typedef double os_double_t;

/** \brief Enumerates the severity level of log messages.
 *
 * \see OS_WINLINDefaultLogFunction()
 */
enum os_winlinLogMessageSeverity_e
{
	OS_WINLIN_LOG_DEBUG = 0,	/**< For debugging log messages. */
	OS_WINLIN_LOG_INFO,			/**< For informative log messages. */
	OS_WINLIN_LOG_WARNING,		/**< For log messages representing warnings. */
	OS_WINLIN_LOG_ERROR,		/**< For error log messages. */
	OS_WINLIN_LOG_DISABLE		/**< To disable logging completely. */
};
/** \brief The type to represent log message severities. */
typedef enum os_winlinLogMessageSeverity_e os_winlinLogMessageSeverity_t;

#endif /* !OS_ASM */

#ifdef __cplusplus
}
#endif

#endif	/* OS_TYPES_WINLIN_H */

/* Editor settings - DO NOT DELETE
 * vi:set ts=4:
 */
