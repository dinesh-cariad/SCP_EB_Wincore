/**
 * \file
 *
 * \brief AUTOSAR LinIf
 *
 * This file contains the implementation of the AUTOSAR
 * module LinIf.
 *
 * \version 5.8.27
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

#include <Std_Types.h>              /* AUTOSAR standard type declarations */
#include <TSAutosar.h>              /* EB specific standard types */
#include <LinIf_Version.h>          /* LinIf version info */

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification check]-------------------*/

#ifndef LINIF_VENDOR_ID /* configuration check */
#error LINIF_VENDOR_ID must be defined
#endif

#if (LINIF_VENDOR_ID != 1U) /* vendor check */
#error LINIF_VENDOR_ID has wrong vendor id
#endif

/*------------------[AUTOSAR release version identification check]----------*/

#ifndef LINIF_AR_RELEASE_MAJOR_VERSION /* configuration check */
#error LINIF_AR_RELEASE_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (LINIF_AR_RELEASE_MAJOR_VERSION != 4U)
#error LINIF_AR_RELEASE_MAJOR_VERSION wrong (!= 4U)
#endif

#ifndef LINIF_AR_RELEASE_MINOR_VERSION /* configuration check */
#error LINIF_AR_RELEASE_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (LINIF_AR_RELEASE_MINOR_VERSION != 0U )
#error LINIF_AR_RELEASE_MINOR_VERSION wrong (!= 0U)
#endif

#ifndef LINIF_AR_RELEASE_REVISION_VERSION /* configuration check */
#error LINIF_AR_RELEASE_REVISION_VERSION must be defined
#endif

/* revision version check */
#if (LINIF_AR_RELEASE_REVISION_VERSION != 3U )
#error LINIF_AR_RELEASE_REVISION_VERSION wrong (!= 3U)
#endif

/*------------------[AUTOSAR module version identification check]-----------*/

#ifndef LINIF_SW_MAJOR_VERSION /* configuration check */
#error LINIF_SW_MAJOR_VERSION must be defined
#endif

/* major version check */
#if (LINIF_SW_MAJOR_VERSION != 5U)
#error LINIF_SW_MAJOR_VERSION wrong (!= 5U)
#endif

#ifndef LINIF_SW_MINOR_VERSION /* configuration check */
#error LINIF_SW_MINOR_VERSION must be defined
#endif

/* minor version check */
#if (LINIF_SW_MINOR_VERSION < 8U)
#error LINIF_SW_MINOR_VERSION wrong (< 8U)
#endif

#ifndef LINIF_SW_PATCH_VERSION /* configuration check */
#error LINIF_SW_PATCH_VERSION must be defined
#endif

/* patch version check */
#if (LINIF_SW_PATCH_VERSION < 27U)
#error LINIF_SW_PATCH_VERSION wrong (< 27U)
#endif

/* prevent empty translation unit in case LinIf_Merged.c is not used */
#if (TS_MERGED_COMPILE == STD_OFF)

#include <TSCompiler.h>
TS_PREVENTEMPTYTRANSLATIONUNIT

#endif /* TS_MERGED_COMPILE */

/*==================[end of file]============================================*/
