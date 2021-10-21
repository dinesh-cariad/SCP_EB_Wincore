/* !LINKSTO BSWM0146,1 */
#ifndef BSWM_VERSION_H
#define BSWM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR BswM
 *
 * This file contains the implementation of the AUTOSAR
 * module BswM.
 *
 * \version 1.15.6
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

#if (defined BSWM_VENDOR_ID)
#error BSWM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define BSWM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined BSWM_MODULE_ID)
#error BSWM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define BSWM_MODULE_ID         42U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined BSWM_AR_RELEASE_MAJOR_VERSION)
#error BSWM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define BSWM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined BSWM_AR_RELEASE_MINOR_VERSION)
#error BSWM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define BSWM_AR_RELEASE_MINOR_VERSION     0U

#if (defined BSWM_AR_RELEASE_REVISION_VERSION)
#error BSWM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define BSWM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined BSWM_SW_MAJOR_VERSION)
#error BSWM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define BSWM_SW_MAJOR_VERSION             1U

#if (defined BSWM_SW_MINOR_VERSION)
#error BSWM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define BSWM_SW_MINOR_VERSION             15U

#if (defined BSWM_SW_PATCH_VERSION)
#error BSWM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define BSWM_SW_PATCH_VERSION             6U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef BSWM_VERSION_H */
/*==================[end of file]============================================*/
