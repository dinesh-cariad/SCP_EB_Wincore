#ifndef DOIP_VERSION_H
#define DOIP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR DoIP
 *
 * This file contains the implementation of the AUTOSAR
 * module DoIP.
 *
 * \version 1.1.21
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

#if (defined DOIP_VENDOR_ID)
#error DOIP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define DOIP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined DOIP_MODULE_ID)
#error DOIP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define DOIP_MODULE_ID         173U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined DOIP_AR_RELEASE_MAJOR_VERSION)
#error DOIP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define DOIP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined DOIP_AR_RELEASE_MINOR_VERSION)
#error DOIP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define DOIP_AR_RELEASE_MINOR_VERSION     1U

#if (defined DOIP_AR_RELEASE_REVISION_VERSION)
#error DOIP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define DOIP_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined DOIP_SW_MAJOR_VERSION)
#error DOIP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define DOIP_SW_MAJOR_VERSION             1U

#if (defined DOIP_SW_MINOR_VERSION)
#error DOIP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define DOIP_SW_MINOR_VERSION             1U

#if (defined DOIP_SW_PATCH_VERSION)
#error DOIP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define DOIP_SW_PATCH_VERSION             21U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef DOIP_VERSION_H */
/*==================[end of file]============================================*/
