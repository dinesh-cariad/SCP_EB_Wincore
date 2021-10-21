#ifndef CANNM_VERSION_H
#define CANNM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR CanNm
 *
 * This file contains the implementation of the AUTOSAR
 * module CanNm.
 *
 * \version 6.19.7
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

#if (defined CANNM_VENDOR_ID)
#error CANNM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CANNM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CANNM_MODULE_ID)
#error CANNM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define CANNM_MODULE_ID         31U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined CANNM_AR_RELEASE_MAJOR_VERSION)
#error CANNM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define CANNM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined CANNM_AR_RELEASE_MINOR_VERSION)
#error CANNM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define CANNM_AR_RELEASE_MINOR_VERSION     0U

#if (defined CANNM_AR_RELEASE_REVISION_VERSION)
#error CANNM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define CANNM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CANNM_SW_MAJOR_VERSION)
#error CANNM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define CANNM_SW_MAJOR_VERSION             6U

#if (defined CANNM_SW_MINOR_VERSION)
#error CANNM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define CANNM_SW_MINOR_VERSION             19U

#if (defined CANNM_SW_PATCH_VERSION)
#error CANNM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define CANNM_SW_PATCH_VERSION             7U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANNM_VERSION_H */
/*==================[end of file]============================================*/
