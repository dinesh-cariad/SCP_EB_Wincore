#ifndef FRIF_VERSION_H
#define FRIF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR FrIf
 *
 * This file contains the implementation of the AUTOSAR
 * module FrIf.
 *
 * \version 5.3.25
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/


#include <FrIf_Version.guards>
/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined FRIF_VENDOR_ID)
#error FRIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FRIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FRIF_MODULE_ID)
#error FRIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FRIF_MODULE_ID         61U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FRIF_AR_RELEASE_MAJOR_VERSION)
#error FRIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FRIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FRIF_AR_RELEASE_MINOR_VERSION)
#error FRIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FRIF_AR_RELEASE_MINOR_VERSION     0U

#if (defined FRIF_AR_RELEASE_REVISION_VERSION)
#error FRIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FRIF_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FRIF_SW_MAJOR_VERSION)
#error FRIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FRIF_SW_MAJOR_VERSION             5U

#if (defined FRIF_SW_MINOR_VERSION)
#error FRIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FRIF_SW_MINOR_VERSION             3U

#if (defined FRIF_SW_PATCH_VERSION)
#error FRIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FRIF_SW_PATCH_VERSION             25U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRIF_VERSION_H */
/*==================[end of file]============================================*/
