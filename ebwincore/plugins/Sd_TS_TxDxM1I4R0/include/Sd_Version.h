#ifndef SD_VERSION_H
#define SD_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Sd
 *
 * This file contains the implementation of the AUTOSAR
 * module Sd.
 *
 * \version 1.4.9
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

#if (defined SD_VENDOR_ID)
#error SD_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define SD_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined SD_MODULE_ID)
#error SD_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define SD_MODULE_ID         171U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined SD_AR_RELEASE_MAJOR_VERSION)
#error SD_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define SD_AR_RELEASE_MAJOR_VERSION     4U

#if (defined SD_AR_RELEASE_MINOR_VERSION)
#error SD_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define SD_AR_RELEASE_MINOR_VERSION     2U

#if (defined SD_AR_RELEASE_REVISION_VERSION)
#error SD_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define SD_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined SD_SW_MAJOR_VERSION)
#error SD_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define SD_SW_MAJOR_VERSION             1U

#if (defined SD_SW_MINOR_VERSION)
#error SD_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define SD_SW_MINOR_VERSION             4U

#if (defined SD_SW_PATCH_VERSION)
#error SD_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define SD_SW_PATCH_VERSION             9U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef SD_VERSION_H */
/*==================[end of file]============================================*/
