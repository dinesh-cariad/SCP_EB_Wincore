#ifndef WDGIF_VERSION_H
#define WDGIF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR WdgIf
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgIf.
 *
 * \version 6.1.26
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

#if (defined WDGIF_VENDOR_ID)
#error WDGIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define WDGIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined WDGIF_MODULE_ID)
#error WDGIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define WDGIF_MODULE_ID         43U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined WDGIF_AR_RELEASE_MAJOR_VERSION)
#error WDGIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define WDGIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined WDGIF_AR_RELEASE_MINOR_VERSION)
#error WDGIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define WDGIF_AR_RELEASE_MINOR_VERSION     0U

#if (defined WDGIF_AR_RELEASE_REVISION_VERSION)
#error WDGIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define WDGIF_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined WDGIF_SW_MAJOR_VERSION)
#error WDGIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define WDGIF_SW_MAJOR_VERSION             6U

#if (defined WDGIF_SW_MINOR_VERSION)
#error WDGIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define WDGIF_SW_MINOR_VERSION             1U

#if (defined WDGIF_SW_PATCH_VERSION)
#error WDGIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define WDGIF_SW_PATCH_VERSION             26U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef WDGIF_VERSION_H */
/*==================[end of file]============================================*/
