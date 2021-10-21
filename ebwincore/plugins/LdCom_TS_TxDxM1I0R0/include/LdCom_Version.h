#ifndef LDCOM_VERSION_H
#define LDCOM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR LdCom
 *
 * This file contains the implementation of the AUTOSAR
 * module LdCom.
 *
 * \version 1.0.26
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

#if (defined LDCOM_VENDOR_ID)
#error LDCOM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define LDCOM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined LDCOM_MODULE_ID)
#error LDCOM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define LDCOM_MODULE_ID         49U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined LDCOM_AR_RELEASE_MAJOR_VERSION)
#error LDCOM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define LDCOM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined LDCOM_AR_RELEASE_MINOR_VERSION)
#error LDCOM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define LDCOM_AR_RELEASE_MINOR_VERSION     0U

#if (defined LDCOM_AR_RELEASE_REVISION_VERSION)
#error LDCOM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define LDCOM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined LDCOM_SW_MAJOR_VERSION)
#error LDCOM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define LDCOM_SW_MAJOR_VERSION             1U

#if (defined LDCOM_SW_MINOR_VERSION)
#error LDCOM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define LDCOM_SW_MINOR_VERSION             0U

#if (defined LDCOM_SW_PATCH_VERSION)
#error LDCOM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define LDCOM_SW_PATCH_VERSION             26U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef LDCOM_VERSION_H */
/*==================[end of file]============================================*/
