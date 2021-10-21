#ifndef CANTP_VERSION_H
#define CANTP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR CanTp
 *
 * This file contains the implementation of the AUTOSAR
 * module CanTp.
 *
 * \version 6.8.42
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

#if (defined CANTP_VENDOR_ID)
#error CANTP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CANTP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CANTP_MODULE_ID)
#error CANTP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define CANTP_MODULE_ID         35U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined CANTP_AR_RELEASE_MAJOR_VERSION)
#error CANTP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define CANTP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined CANTP_AR_RELEASE_MINOR_VERSION)
#error CANTP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define CANTP_AR_RELEASE_MINOR_VERSION     0U

#if (defined CANTP_AR_RELEASE_REVISION_VERSION)
#error CANTP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define CANTP_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CANTP_SW_MAJOR_VERSION)
#error CANTP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define CANTP_SW_MAJOR_VERSION             6U

#if (defined CANTP_SW_MINOR_VERSION)
#error CANTP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define CANTP_SW_MINOR_VERSION             8U

#if (defined CANTP_SW_PATCH_VERSION)
#error CANTP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define CANTP_SW_PATCH_VERSION             42U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANTP_VERSION_H */
/*==================[end of file]============================================*/
