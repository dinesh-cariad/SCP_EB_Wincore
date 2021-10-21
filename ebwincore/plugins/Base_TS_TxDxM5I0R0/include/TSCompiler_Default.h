#ifndef TS_COMPILER_DEFAULT_H
#define TS_COMPILER_DEFAULT_H

/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */
/*  MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 20.10 (advisory)
 * The # and ## preprocessor operators should not be used.
 *
 * Reason:
 * There is no other way to concatenate identifiers, which is needed for specific code patterns.
 *
 * MISRAC2012-2) Deviated Rule: 20.11 (required)
 * A macro parameter immediately following a # operator shall not
 * immediately be followed by a ## operator
 *
 * Reason:
 * This rule is necessary to avoid side effects, as the evaluation order is undefined.
 * In this case, the two uses of ## do not contribute to the same identifier,
 * so evaluation order is not an issue.
 *
 */

/*==================[macros]=================================================*/

#define UNIQUETYPEDEF(x, y) x ## y
#define UNIQUE(x, y) UNIQUETYPEDEF(x, y)

/** \brief Prevent empty translation unit
 *
 * This macro shall be used to prevent that a translation unit is empty.
 */
#if ((defined TS_MERGED_COMPILE) && (defined STD_ON))
#if (TS_MERGED_COMPILE == STD_ON)
#define TS_PREVENTEMPTYTRANSLATIONUNIT
#else /* (TS_MERGED_COMPILE == STD_ON) */
#define TS_PREVENTEMPTYTRANSLATIONUNIT \
    typedef void UNIQUE(TSPreventEmptyTranslationUnit, __LINE__);
#endif /* (TS_MERGED_COMPILE == STD_ON) */
#else /* (defined TS_MERGED_COMPILE) && (defined STD_ON) */
#define TS_PREVENTEMPTYTRANSLATIONUNIT \
    typedef void UNIQUE(TSPreventEmptyTranslationUnit, __LINE__);
#endif /* (defined TS_MERGED_COMPILE) && (defined STD_ON) */

#if (defined TS_DEPRECATED_TYPEDEF)  /* to prevent double definition */
#error TS_DEPRECATED_TYPEDEF is already defined
#endif /* if (defined TS_DEPRECATED_TYPEDEF) */

#if (defined __GNUC__)
/** \brief Tag a typedef as deprecated
 *
 * This macro shall be used to tag a typedef as deprecated.
 *
 * Example use:
 *
 * typedef struct
 * {
 *     uint8 member0;
 *     uint8 member1;
 * } deprecatedType TS_DEPRECATED_TYPEDEF;
 *
 * This yields the following warning for every _USE_ of the typedef
 * during compilation:
 *
 * appl.c: In function 'TestDeprecatedTypedef':
 * appl.c:84: warning: 'deprecatedType' is deprecated
 *
 */
/* disable CheckC warning about two consecutive underscores */
/* CHECK: RULE 501 OFF */
#define TS_DEPRECATED_TYPEDEF __attribute__ ((deprecated))
/* CHECK: RULE 501 ON */
#else /* if (defined __GNUC__) */
#define TS_DEPRECATED_TYPEDEF
#endif /* if (defined __GNUC__) */

#if (defined TS_DEPRECATED_VARIABLE)  /* to prevent double definition */
#error TS_DEPRECATED_VARIABLE is already defined
#endif /* if (defined TS_DEPRECATED_VARIABLE) */

#if (defined __GNUC__)
/** \brief Tag a variable as deprecated
 *
 * This macro shall be used to tag a variable as deprecated.
 *
 * Example use:
 *
 * uint8 deprecatedVariable TS_DEPRECATED_VARIABLE;
 *
 * This yields the following warning for every _USE_ of the variable
 * during compilation:
 *
 * appl.c: In function 'TestDeprecatedVariable':
 * appl.c:91: warning: 'deprecatedVariable' is deprecated (declared at appl.c:48)
 *
 */
/* disable CheckC warning about two consecutive underscores */
/* CHECK: RULE 501 OFF */
#define TS_DEPRECATED_VARIABLE __attribute__ ((deprecated))
/* CHECK: RULE 501 ON */
#else /* if (defined __GNUC__) */
#define TS_DEPRECATED_VARIABLE
#endif /* if (defined __GNUC__) */

#if (defined TS_DEPRECATED_FUNCTION)  /* to prevent double definition */
#error TS_DEPRECATED_FUNCTION is already defined
#endif /* if (defined TS_DEPRECATED_FUNCTION) */

#if (defined __GNUC__)
/** \brief Tag a function as deprecated
 *
 * This macro shall be used to tag a function as deprecated.
 *
 * Example use:
 *
 * STATIC FUNC(void, EBTEST_CODE) TestDeprecatedFunction(void) TS_DEPRECATED_FUNCTION;
 *
 * This yields the following warning for every _USE_ of the function
 * during compilation:
 *
 * appl.c: In function 'TestMain':
 * appl.c:62: warning: 'TestDeprecatedFunction' is deprecated (declared at appl.c:29)
 *
 */
/* disable CheckC warning about two consecutive underscores */
/* CHECK: RULE 501 OFF */
#define TS_DEPRECATED_FUNCTION __attribute__ ((deprecated))
/* CHECK: RULE 501 ON */
#else /* if (defined __GNUC__) */
#define TS_DEPRECATED_FUNCTION
#endif /* if (defined __GNUC__) */

#if (defined TS_DOPRAGMA) /* to prevent double definition */
#error TS_DOPRAGMA is already defined
#endif /* if (defined TS_DOPRAGMA) */

#if (defined __GNUC__)
/** \brief Helper macro for other deprecation macros */
#define TS_DOPRAGMA(x) _Pragma (#x)
#else /* if (defined __GNUC__) */
#define TS_DOPRAGMA(x)
#endif /* if (defined __GNUC__) */

#if (defined TS_DEPRECATED_MACRO)  /* to prevent double definition */
#error TS_DEPRECATED_MACRO is already defined
#endif /* if (defined TS_DEPRECATED_MACRO) */

#if (defined __GNUC__)
/** \brief Tag a macro as deprecated
 *
 * This macro shall be used to tag a macro as deprecated.
 *
 * Example use:
 *
 * #define DEPRECATED_MACRO(a,b)                       \
 *     do                                              \
 *     {                                               \
 *         TS_DEPRECATED_MACRO(DEPRECATED_MACRO)       \
 *         (a) = (b);                                  \
 *     } while(0)
 *
 * This yields the following warning for every _USE_ of the macro
 * during compilation:
 *
 * appl.c: In function 'TestDeprecatedMacro':
 * appl.c:99: warning: ignoring #pragma Macro_DEPRECATED_MACRO_is_deprecated
 *
 * Note that this warning that the #pragma is ignored is the best we can do, since the only
 * means gcc provides to explicitly trigger warning messages are #error and #warning, which both
 * cannot be used in macros. - Note further that #pragma message is not supported by gcc (although
 * the docu states that is is).
 */
#define TS_DEPRECATED_MACRO(x) \
  TS_DOPRAGMA(Macro_ ## x ## _is_deprecated)
#else /* if (defined __GNUC__) */
#define TS_DEPRECATED_MACRO(x)
#endif /* if (defined __GNUC__) */


#ifndef TS_CAST
/** \brief Cast \a expr to the type given by \a type and \a bits.
 *
 * usage: e.g:  TS_CAST(uint, 8, ((length) >> 8u))
 *
 * This macro is used to suppress excessive warnings from some compilers,
 * while keeping the code readable and efficient for well-behaving compilers.
 *
 * Note: Using the default version with the C cast should be sufficient for all compilers.
 * Unless your compiler misbehaves, don't overrride the default definition.
 *
 * Background:
 *
 * There are many ways for a compiler to do sign/zero extensions:
 * shift back & forth, AND with a constant, move between registers of different sizes,
 * explicit sign/unsigned-extend instructions, bit-insertion instructions, ...
 *
 * Which route is the cheapest, will depend on the architecture, size of the masking constant, the
 * location of the variable within the register and maybe even on other constants in the program,
 * or the constants currently residing in registers.
 *
 * Your compiler may select any - and may not even find the cheapest, depending on strategy,
 * phase-ordering within the compiler, abortion due to compile time limits, ...
 *
 * Add the commutative property of AND, throw in adjacent shifts that may be joined with shifts in
 * the same direction, and the peep-hole optimizer will have a hard time identifying all possible
 * expression tree patterns to eliminate the redundant sign/zero extension with a given compile
 * time budget.
 *
 * Now, if you still want to proceed, you may need to insert one of the following variants in the
 * Compiler.h for your compiler/architecture, in order to appease your specific compiler -
 * adding Deviation MISRAC2012-2 to the set of deviations:
 *
 * #define TS_CAST(type, bits, expr)  ((type##bits)((expr) & ((type##bits)(~((type##bits)0u)))))
 *
 * #define TS_CAST(type, bits, expr)  ((type##bits)((expr) & ((type##bits)-1)))
 *
 */
/* Deviation MISRAC2012-1 */
#define TS_CAST(type, bits, expr)  ((type##bits)(expr))
#endif /* ifndef TS_CAST */

/*==================[end of file]============================================*/
#endif /* ifndef TS_COMPILER_DEFAULT_H */
