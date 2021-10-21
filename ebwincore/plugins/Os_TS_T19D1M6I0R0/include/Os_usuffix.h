/* Os_usuffix.h
 *
 * Definition of a macro to suffix constant integers with 'U' to
 * mark them as unsigned numbers. This allows their use in assembler
 * as well as C code, because the 'U' suffix is disabled in the former
 * and enabled in the latter case.
 *
 * CHECK: TABS 4 (see editor commands at end of file)
 *
 * Copyright Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 *
 * $Id: Os_usuffix.h 27282 2017-08-24 13:57:43Z olme8414 $
*/

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * Necessary to share the same code between assembler and C compiler.
 */

#ifndef OS_USUFFIX_H_
#define OS_USUFFIX_H_

/** \def OS_UU
 * \brief Helper of OS_U. Needed for proper operation of \c ## preprocessor operator.
 */
#ifndef OS_ASM
/* Deviation MISRAC2012-1 */
#define OS_UU(x) x ## U
#else
#define OS_UU(x) x
#endif

/** \brief Appends the character 'U' to \a x to mark it as \e unsigned.
 *
 * This macro allows to use the same definition of a constant number in C
 * and assembler code.
 * Some assemblers don't like the 'U' suffix and cease to work.
 * The 'U', though, is important to suppress MISRA warnings.
 * This macro removes the 'U', when the code is prepared (preprocessed) for an assembler
 * and preserves it for a C compiler. This decision is based on the macro OS_ASM.
 *
 * \param x  A constant number.
 *
 * \see OS_UU
 */
#define OS_U(x) OS_UU(x)

#endif /* OS_USUFFIX_H_ */

/* Editor settings: DO NOT DELETE
 * vi:set ts=4:
*/
