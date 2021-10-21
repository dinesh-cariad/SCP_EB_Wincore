#ifndef SOAD_VERSION_H
#define SOAD_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR SoAd
 *
 * This file contains the implementation of the AUTOSAR
 * module SoAd.
 *
 * \version 1.8.16
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

#if (defined SOAD_VENDOR_ID)
#error SOAD_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define SOAD_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined SOAD_MODULE_ID)
#error SOAD_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define SOAD_MODULE_ID         56U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined SOAD_AR_RELEASE_MAJOR_VERSION)
#error SOAD_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define SOAD_AR_RELEASE_MAJOR_VERSION     4U

#if (defined SOAD_AR_RELEASE_MINOR_VERSION)
#error SOAD_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define SOAD_AR_RELEASE_MINOR_VERSION     2U

#if (defined SOAD_AR_RELEASE_REVISION_VERSION)
#error SOAD_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define SOAD_AR_RELEASE_REVISION_VERSION  2U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined SOAD_SW_MAJOR_VERSION)
#error SOAD_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define SOAD_SW_MAJOR_VERSION             1U

#if (defined SOAD_SW_MINOR_VERSION)
#error SOAD_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define SOAD_SW_MINOR_VERSION             8U

#if (defined SOAD_SW_PATCH_VERSION)
#error SOAD_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define SOAD_SW_PATCH_VERSION             16U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef SOAD_VERSION_H */
/*==================[end of file]============================================*/
