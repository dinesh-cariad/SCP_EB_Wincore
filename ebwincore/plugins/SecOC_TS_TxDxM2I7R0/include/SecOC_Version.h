#ifndef SECOC_VERSION_H
#define SECOC_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR SecOC
 *
 * This file contains the implementation of the AUTOSAR
 * module SecOC.
 *
 * \version 2.7.4
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

#if (defined SECOC_VENDOR_ID)
#error SECOC_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define SECOC_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined SECOC_MODULE_ID)
#error SECOC_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define SECOC_MODULE_ID         607U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined SECOC_AR_RELEASE_MAJOR_VERSION)
#error SECOC_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define SECOC_AR_RELEASE_MAJOR_VERSION     4U

#if (defined SECOC_AR_RELEASE_MINOR_VERSION)
#error SECOC_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define SECOC_AR_RELEASE_MINOR_VERSION     3U

#if (defined SECOC_AR_RELEASE_REVISION_VERSION)
#error SECOC_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define SECOC_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined SECOC_SW_MAJOR_VERSION)
#error SECOC_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define SECOC_SW_MAJOR_VERSION             2U

#if (defined SECOC_SW_MINOR_VERSION)
#error SECOC_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define SECOC_SW_MINOR_VERSION             7U

#if (defined SECOC_SW_PATCH_VERSION)
#error SECOC_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define SECOC_SW_PATCH_VERSION             4U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef SECOC_VERSION_H */
/*==================[end of file]============================================*/

