#ifndef FRARTP_VERSION_H
#define FRARTP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR FrArTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrArTp.
 *
 * \version 1.0.8
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

#if (defined FRARTP_VENDOR_ID)
#error FRARTP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FRARTP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FRARTP_MODULE_ID)
#error FRARTP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FRARTP_MODULE_ID         38U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FRARTP_AR_RELEASE_MAJOR_VERSION)
#error FRARTP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FRARTP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FRARTP_AR_RELEASE_MINOR_VERSION)
#error FRARTP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FRARTP_AR_RELEASE_MINOR_VERSION     2U

#if (defined FRARTP_AR_RELEASE_REVISION_VERSION)
#error FRARTP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FRARTP_AR_RELEASE_REVISION_VERSION  2U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FRARTP_SW_MAJOR_VERSION)
#error FRARTP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FRARTP_SW_MAJOR_VERSION             1U

#if (defined FRARTP_SW_MINOR_VERSION)
#error FRARTP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FRARTP_SW_MINOR_VERSION             0U

#if (defined FRARTP_SW_PATCH_VERSION)
#error FRARTP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FRARTP_SW_PATCH_VERSION             8U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRARTP_VERSION_H */
/*==================[end of file]============================================*/
