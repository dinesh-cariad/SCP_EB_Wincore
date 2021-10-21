#ifndef LINIF_VERSION_H
#define LINIF_VERSION_H
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

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined LINIF_VENDOR_ID)
#error LINIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define LINIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined LINIF_MODULE_ID)
#error LINIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define LINIF_MODULE_ID         62U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined LINIF_AR_RELEASE_MAJOR_VERSION)
#error LINIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define LINIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined LINIF_AR_RELEASE_MINOR_VERSION)
#error LINIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define LINIF_AR_RELEASE_MINOR_VERSION     0U

#if (defined LINIF_AR_RELEASE_REVISION_VERSION)
#error LINIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define LINIF_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined LINIF_SW_MAJOR_VERSION)
#error LINIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define LINIF_SW_MAJOR_VERSION             5U

#if (defined LINIF_SW_MINOR_VERSION)
#error LINIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define LINIF_SW_MINOR_VERSION             8U

#if (defined LINIF_SW_PATCH_VERSION)
#error LINIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define LINIF_SW_PATCH_VERSION             27U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef LINIF_VERSION_H */
/*==================[end of file]============================================*/
