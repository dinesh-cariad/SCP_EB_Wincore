#ifndef FRNM_VERSION_H
#define FRNM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR FrNm
 *
 * This file contains the implementation of the AUTOSAR
 * module FrNm.
 *
 * \version 5.16.7
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

#if (defined FRNM_VENDOR_ID)
#error FRNM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FRNM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FRNM_MODULE_ID)
#error FRNM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FRNM_MODULE_ID         32U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FRNM_AR_RELEASE_MAJOR_VERSION)
#error FRNM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FRNM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FRNM_AR_RELEASE_MINOR_VERSION)
#error FRNM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FRNM_AR_RELEASE_MINOR_VERSION     0U

#if (defined FRNM_AR_RELEASE_REVISION_VERSION)
#error FRNM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FRNM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FRNM_SW_MAJOR_VERSION)
#error FRNM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FRNM_SW_MAJOR_VERSION             5U

#if (defined FRNM_SW_MINOR_VERSION)
#error FRNM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FRNM_SW_MINOR_VERSION             16U

#if (defined FRNM_SW_PATCH_VERSION)
#error FRNM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FRNM_SW_PATCH_VERSION             7U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRNM_VERSION_H */
/*==================[end of file]============================================*/
