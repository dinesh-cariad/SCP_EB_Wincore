/* Os_tool_WINDOWS_gnu.h
 *
 * Definitions for GNU toolchain on WINDOWS.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_tool_WINDOWS_gnu.h 27664 2017-10-19 08:25:50Z olme8414 $
*/

/*	This file contains macros (C and assembler) for use with Windows
 *	and the gcc compiler.
*/

#ifndef OS_TOOL_WINDOWS_GNU_H
#define OS_TOOL_WINDOWS_GNU_H

#define OS_TOOL_INLINE_KEYWORD __inline__
#define OS_TOOL_ASM_KEYWORD __asm__

/** \name Calling Convention Decorators.
 *
 * \see http://gcc.gnu.org/onlinedocs/gcc-4.7.2/gcc/Function-Attributes.html#Function-Attributes
 */
/** @{ */
#define OS_WINDOWS_STDCALL __attribute__((stdcall))
#define OS_WINDOWS_CDECL __attribute__((cdecl))
/** @} */

/** \brief Checks, if the GCC compiler has at least the given version.
 *
 * \see http://gcc.gnu.org/onlinedocs/cpp/Common-Predefined-Macros.html
 *
 * \param major			The major part of the version.
 * \param minor			The minor part of the version.
 * \param patchlevel	The patchlevel.
 *
 * \retval OS_TRUE	if the version of the compiler is at least \a major.\a minor.\a patchlevel.
 * \retval OS_FALSE otherwise.
 */
#define OS_GccVersionIsAtLeast(major, minor, patchlevel)	\
	((__GNUC__ > (major)) ||								\
		((__GNUC__ == (major)) &&							\
			((__GNUC_MINOR__ > (minor)) ||					\
				((__GNUC_MINOR__ == (minor)) && (__GNUC_PATCHLEVEL__ >= (patchlevel)))	\
			)												\
		)													\
	)

/** \brief Generates a definition of a stack.
 *
 * \remarks Microsoft Windows distrusts programs with too many sections. The test case
 * TestSuite4/Testcases/Blackbox/OSEKOS2.2/TaskManagement/tmts4 fails, because the compiled executable
 * is claimed to be invalid by the Microsoft Windows loader. With the following alternative macro definition
 * the test case finishes successfully, though.
 * Furthermore, on Microsoft Windows it's OK to put all stacks into one .bss section, because there is no
 * linker script, which could make use of those special section annotations.
 *
 * \see Macro definition in asc_Os/src/Autosar/include/Os_tool.h
 */
#define OS_GEN_STACK(var, sec, size, granule) os_stackelement_t (var)[(size)]

#endif	/* OS_TOOL_WINDOWS_GNU_H */

/* Editor settings; DO NOT DELETE
 * vi:set ts=4:
*/
