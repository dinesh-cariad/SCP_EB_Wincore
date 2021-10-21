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
 * The macro _GREENHILLS_C_ARM_ is only used for Autosar 2.1 backward compatibility.
 */
/* COMPILER010 */
/* !LINKSTO CompilerAbstraction.ASR403.COMPILER010, 1 */
#ifndef _GREENHILLS_C_ARM_ /* to prevent double definition */
/** \brief compiler symbol for Greenhills compiler for ARM*/
/* Deviation MISRAC2012-1 */
#define _GREENHILLS_C_ARM_
#else
#error _GREENHILLS_C_ARM_ already defined
#endif /* ifndef _GREENHILLS_C_ARM_ */

#endif /* ifndef COMPILER_SPECIFIC_H */
