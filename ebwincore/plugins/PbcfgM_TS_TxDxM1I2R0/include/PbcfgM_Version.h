/* !LINKSTO PbcfgM.EB.PBCFGM403, 1 */
#ifndef PBCFGM_VERSION_H
#define PBCFGM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR PbcfgM
 *
 * This file contains the implementation of the AUTOSAR
 * module PbcfgM.
 *
 * \version 1.2.22
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

#if (defined PBCFGM_VENDOR_ID)
#error PBCFGM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define PBCFGM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined PBCFGM_MODULE_ID)
#error PBCFGM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define PBCFGM_MODULE_ID         32769U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined PBCFGM_AR_RELEASE_MAJOR_VERSION)
#error PBCFGM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define PBCFGM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined PBCFGM_AR_RELEASE_MINOR_VERSION)
#error PBCFGM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define PBCFGM_AR_RELEASE_MINOR_VERSION     0U

#if (defined PBCFGM_AR_RELEASE_REVISION_VERSION)
#error PBCFGM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define PBCFGM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined PBCFGM_SW_MAJOR_VERSION)
#error PBCFGM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define PBCFGM_SW_MAJOR_VERSION             1U

#if (defined PBCFGM_SW_MINOR_VERSION)
#error PBCFGM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define PBCFGM_SW_MINOR_VERSION             2U

#if (defined PBCFGM_SW_PATCH_VERSION)
#error PBCFGM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define PBCFGM_SW_PATCH_VERSION             22U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef PBCFGM_VERSION_H */
/*==================[end of file]============================================*/
