/**
 * \file
 *
 * \brief AUTOSAR E2E
 *
 * This file contains the implementation of the AUTOSAR
 * module E2E.
 *
 * \version 2.0.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

#ifndef E2E_DEFS_H
#define E2E_DEFS_H

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

#if (defined E2E_PARAM_UNUSED)
#error E2E_PARAM_UNUSED already defined
#endif

/** \brief This macro can be used to avoid compiler warnings */
#define E2E_PARAM_UNUSED(x) ((void) (x))


#if (defined E2E_HLP_UNIQUETYPEDEF)
#error E2E_HLP_UNIQUETYPEDEF already defined
#endif

#if (defined E2E_HLP_UNIQUE)
#error E2E_HLP_UNIQUE already defined
#endif

/* Helper macros for string concatination */
#define E2E_HLP_UNIQUETYPEDEF(x, y) x ## y
#define E2E_HLP_UNIQUE(x, y) E2E_HLP_UNIQUETYPEDEF(x, y)

#if (defined E2E_PREVENTEMPTYTRANSLATIONUNIT)
#error E2E_PREVENTEMPTYTRANSLATIONUNIT already defined
#endif

/** \brief Prevent empty translation unit
 *
 * This macro shall be used to prevent that a translation unit is empty.
 */
/* CheckC disabled due to warning about two consecutive underscores.
 * The standard predefined macro is used to satisfy MISRA rule 5.6. */
/* CHECK: RULE 501 OFF */
#define E2E_PREVENTEMPTYTRANSLATIONUNIT \
    typedef void E2E_HLP_UNIQUE(E2EPreventEmptyTranslationUnit, __LINE__)
/* CHECK: RULE 501 ON */

/*==================[type definitions]=======================================*/

/*==================[external constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[external function declarations]=========================*/

#endif /* ifndef E2E_DEFS_H */
/*==================[end of file]============================================*/
