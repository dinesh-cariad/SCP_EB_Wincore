#ifndef COMPILER_CFGINT_H
#define COMPILER_CFGINT_H
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

[!SELECT "CompilerGeneral"!][!//
[!IF "IncludeCompilerCfgExtMcal = 'true'"!][!//
/**  Inclusion of Compiler_CfgExtMcal.h file
 **
 ** En inclusion of Compiler_CfgExtMcal.h file  */
#ifndef COMPILERCFG_EXTENSION_MCAL_FILE
#define COMPILERCFG_EXTENSION_MCAL_FILE
#endif

[!ENDIF!][!//

[!IF "IncludeCompilerCfgExt = 'true'"!][!//
/**  Inclusion of Compiler_CfgExt.h file
 **
 ** En inclusion of Compiler_CfgExt.h file  */
#ifndef COMPILERCFG_EXTENSION_FILE
#define COMPILERCFG_EXTENSION_FILE
#endif
[!ENDIF!][!//
[!ENDSELECT!][!//

#endif /* #if (!defined COMPILER_CFGINT_H) */
/*==================[end of file]============================================*/
