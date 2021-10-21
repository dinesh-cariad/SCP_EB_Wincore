#ifndef COMPILER_COMMON_H
#define COMPILER_COMMON_H

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

/* MISRA-C:2012 Deviation List
 *
 * MISRAC2012-1) Deviated Rule: 21.1 (required)
 * #define and #undef shall not be used on a reserved identifier or reserved
 * macro name.
 *
 * Reason:
 * Generally, all identifiers that begin with the underscore character are reserved.
 * The macro _STATIC_ is only used for Autosar 2.1 backward compatibility
 *
 */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined COMPILER_VENDOR_ID)
#error COMPILER_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define COMPILER_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined COMPILER_MODULE_ID)
#error COMPILER_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define COMPILER_MODULE_ID       198U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined COMPILER_AR_RELEASE_MAJOR_VERSION)
#error COMPILER_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define COMPILER_AR_RELEASE_MAJOR_VERSION     4U

#if (defined COMPILER_AR_RELEASE_MINOR_VERSION)
#error COMPILER_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define COMPILER_AR_RELEASE_MINOR_VERSION     0U

#if (defined COMPILER_AR_RELEASE_REVISION_VERSION)
#error COMPILER_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define COMPILER_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined COMPILER_SW_MAJOR_VERSION)
#error COMPILER_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define COMPILER_SW_MAJOR_VERSION    3U

#if (defined COMPILER_SW_MINOR_VERSION)
#error COMPILER_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define COMPILER_SW_MINOR_VERSION    2U

#if (defined COMPILER_SW_PATCH_VERSION)
#error COMPILER_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define COMPILER_SW_PATCH_VERSION    0U

/*------------------[AUTOSAR module vendor API infix]------------------------*/

#if (defined COMPILER_VENDOR_API_INFIX)
#error COMPILER_VENDOR_API_INFIX already defined
#endif
/** \brief Vendor API infix
 *
 * Left empty as this header does not belong to any module */
#define COMPILER_VENDOR_API_INFIX

/*------------------[storage classes]----------------------------------------*/

#if (defined AUTOMATIC)
#error AUTOMATIC already defined
#endif

/* COMPILER046 */
/** \brief definition of an automatic memory class
 *
 * To be used for local non static variables */
#define AUTOMATIC

#if (defined TYPEDEF)
#error TYPEDEF already defined
#endif

/* COMPILER059 */
/** \brief definition of an type-definition memory class
 *
 * To be used within type definitions only
 */
#define TYPEDEF

#if (defined STATIC)
#error STATIC already defined
#endif

/** \brief abstraction of compiler keyword 'static
 *
 * values: 'static' or empty */
#define STATIC static

#if (defined _STATIC_)
#error _STATIC_ already defined
#endif

/** \brief map _STATIC_ to value of STATIC for Autosar 2.1 backward compatibility */
/* Deviation MISRAC2012-1 */
#define _STATIC_ static

/*------------------[const correctness for function parameters]--------------*/

/** \brief macro to be used for top-level \a const -ness of function parameters.
 *
 * Background: In C, the top level \a const -ness of a parameter identifier is not
 * part of the parameter profile. Hence it is always useful to write-protect incoming
 * parameters, even if they are passed by-value. While this is also useful for scalars
 * (int, etc), the biggest value shows for pointers and pointers to pointers that should
 * not be changed within the function.
 * \para Hint: as the top level const belongs to the identifier itself, making it a non-L-value
 * (not assignable), it makes much sense to write this CONSTDEF immediately in front of the identifier.
 * If a specific implementation needs to change that value (e.g.: redirect to temporary buffer, ...),
 * the implementation is free to leave out that top-level \a const and still satisfies the interface
 * (prototype, function-pointer assignability, linker symbol-lookup, ...)
 * Only DIAB has problems with top level const definitions vs declarations: they must be identical.
 * Hence, for Diab, we define this symbol as empty.
 * General const-correctness and safety is still maintained throughout all other architectures.
 */
#if (defined __DCC__) && (!defined CONSTDEF)
#define CONSTDEF /* empty: no const-correctness for broken DIAB compilers */
#endif
#if !(defined CONSTDEF)
#define CONSTDEF const
#endif

/*------------------[null pointer constant]----------------------------------*/

#if (defined NULL_PTR)
#error NULL_PTR already defined
#endif

/* COMPILER051 */
/** \brief abstraction of the null pointer constant */
#define NULL_PTR ((void *)0)

/*==================[end of file]============================================*/
#endif /* ifndef COMPILER_COMMON_H */
