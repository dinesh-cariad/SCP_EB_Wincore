#ifndef COMPILER_SPECIFIC_H
#define COMPILER_SPECIFIC_H
/* --------{ EB Automotive C Source File }-------- */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*------------------[compiler symbol]----------------------------------------*/

/*
 * MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 21.1 (required)
 * #define and #undef shall not be used on a reserved identifier or reserved
 * macro name.
 *
 * Reason:
 * Generally, all identifiers that begin with the underscore character are reserved.
 * The macro _DIABDATA_C_ARM64_ is only used for Autosar 2.1 backward compatibility.
 */
/* COMPILER010 */
/* !LINKSTO CompilerAbstraction.ASR403.COMPILER010, 1 */
#if (defined _DIABDATA_C_ARM64_) /* to prevent double definition */
#error _DIABDATA_C_ARM64_ already defined
#endif /* if (defined _DIABDATA_C_ARM64_) */

/** \brief compiler symbol for DiabData compiler for PowerPC */
/* Deviation MISRAC2012-1 */
#define _DIABDATA_C_ARM64_ 1U

#endif /* ifndef COMPILER_SPECIFIC_H */
