#ifndef WDGM_VERSION_H
#define WDGM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR WdgM
 *
 * This file contains the implementation of the AUTOSAR
 * module WdgM.
 *
 * \version 6.1.39
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

#if (defined WDGM_VENDOR_ID)
#error WDGM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define WDGM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined WDGM_MODULE_ID)
#error WDGM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define WDGM_MODULE_ID         13U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined WDGM_AR_RELEASE_MAJOR_VERSION)
#error WDGM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define WDGM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined WDGM_AR_RELEASE_MINOR_VERSION)
#error WDGM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define WDGM_AR_RELEASE_MINOR_VERSION     0U

#if (defined WDGM_AR_RELEASE_REVISION_VERSION)
#error WDGM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define WDGM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined WDGM_SW_MAJOR_VERSION)
#error WDGM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define WDGM_SW_MAJOR_VERSION             6U

#if (defined WDGM_SW_MINOR_VERSION)
#error WDGM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define WDGM_SW_MINOR_VERSION             1U

#if (defined WDGM_SW_PATCH_VERSION)
#error WDGM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define WDGM_SW_PATCH_VERSION             39U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef WDGM_VERSION_H */
/*==================[end of file]============================================*/
