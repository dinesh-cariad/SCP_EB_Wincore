#ifndef MIRROR_VERSION_H
#define MIRROR_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Mirror
 *
 * This file contains the implementation of the AUTOSAR
 * module Mirror.
 *
 * \version 1.0.17
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

#if (defined MIRROR_VENDOR_ID)
#error MIRROR_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define MIRROR_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined MIRROR_MODULE_ID)
#error MIRROR_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define MIRROR_MODULE_ID         48U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined MIRROR_AR_RELEASE_MAJOR_VERSION)
#error MIRROR_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define MIRROR_AR_RELEASE_MAJOR_VERSION     4U

#if (defined MIRROR_AR_RELEASE_MINOR_VERSION)
#error MIRROR_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define MIRROR_AR_RELEASE_MINOR_VERSION     3U

#if (defined MIRROR_AR_RELEASE_REVISION_VERSION)
#error MIRROR_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define MIRROR_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined MIRROR_SW_MAJOR_VERSION)
#error MIRROR_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define MIRROR_SW_MAJOR_VERSION             1U

#if (defined MIRROR_SW_MINOR_VERSION)
#error MIRROR_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define MIRROR_SW_MINOR_VERSION             0U

#if (defined MIRROR_SW_PATCH_VERSION)
#error MIRROR_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define MIRROR_SW_PATCH_VERSION             17U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef MIRROR_VERSION_H */
/*==================[end of file]============================================*/
