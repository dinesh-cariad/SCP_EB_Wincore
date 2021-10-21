#ifndef XCPR_VERSION_H
#define XCPR_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR XcpR
 *
 * This file contains the implementation of the AUTOSAR
 * module XcpR.
 *
 * \version 1.1.0
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

#if (defined XCPR_VENDOR_ID)
#error XCPR_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define XCPR_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined XCPR_MODULE_ID)
#error XCPR_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define XCPR_MODULE_ID         255U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined XCPR_AR_RELEASE_MAJOR_VERSION)
#error XCPR_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define XCPR_AR_RELEASE_MAJOR_VERSION     4U

#if (defined XCPR_AR_RELEASE_MINOR_VERSION)
#error XCPR_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define XCPR_AR_RELEASE_MINOR_VERSION     0U

#if (defined XCPR_AR_RELEASE_REVISION_VERSION)
#error XCPR_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define XCPR_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined XCPR_SW_MAJOR_VERSION)
#error XCPR_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define XCPR_SW_MAJOR_VERSION             1U

#if (defined XCPR_SW_MINOR_VERSION)
#error XCPR_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define XCPR_SW_MINOR_VERSION             1U

#if (defined XCPR_SW_PATCH_VERSION)
#error XCPR_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define XCPR_SW_PATCH_VERSION             0U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef XCPR_VERSION_H */
/*==================[end of file]============================================*/
