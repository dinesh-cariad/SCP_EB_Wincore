#ifndef COMPILER_SPECIFIC_H
#define COMPILER_SPECIFIC_H
/**
 * \file
 *
 * \brief AUTOSAR Compiler
 *
 * This file contains the implementation of the AUTOSAR
 * module Compiler.
 *
 * \version 1.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2017 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

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
 * The macro _DIABDATA_C_TRICORE_ is only used for Autosar 2.1 backward compatibility.
 */
/* COMPILER010 */
/* !LINKSTO CompilerAbstraction.ASR403.COMPILER010, 1 */
#ifndef _DIABDATA_C_TRICORE_ /* to prevent double definition */
/** \brief compiler symbol for DiabData compiler for TRICORE */
/* Deviation MISRAC2012-1 */
#define _DIABDATA_C_TRICORE_
#else
#error _DIABDATA_C_TRICORE_ already defined
#endif /* ifndef _DIABDATA_C_TRICORE_ */

/* COMPILER057 */
/* EB changed adding inline keywords cause for Infineon MCALs at TC29XT it is mandatory *
 * Tracked in ticket: ASCINFINEON-507 */
/*CHECK: RULE 501 OFF */
/* check C Rule 501: Two consecutive Underscores are used
   __inline__ is a Diab compiler specific keyword */
/* Deviation MISRAC2012-1 */
#define COMPILER_SPECIFIC_INLINE __inline__
/*CHECK: RULE 501 ON */

#endif /* ifndef COMPILER_SPECIFIC_H */
