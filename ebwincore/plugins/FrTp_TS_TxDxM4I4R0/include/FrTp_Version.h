#ifndef FRTP_VERSION_H
#define FRTP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR FrTp
 *
 * This file contains the implementation of the AUTOSAR
 * module FrTp.
 *
 * \version 4.4.26
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/


#include <FrTp_Version.guards>
/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined FRTP_VENDOR_ID)
#error FRTP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FRTP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FRTP_MODULE_ID)
#error FRTP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FRTP_MODULE_ID         36U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FRTP_AR_RELEASE_MAJOR_VERSION)
#error FRTP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FRTP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FRTP_AR_RELEASE_MINOR_VERSION)
#error FRTP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FRTP_AR_RELEASE_MINOR_VERSION     0U

#if (defined FRTP_AR_RELEASE_REVISION_VERSION)
#error FRTP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FRTP_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FRTP_SW_MAJOR_VERSION)
#error FRTP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FRTP_SW_MAJOR_VERSION             4U

#if (defined FRTP_SW_MINOR_VERSION)
#error FRTP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FRTP_SW_MINOR_VERSION             4U

#if (defined FRTP_SW_PATCH_VERSION)
#error FRTP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FRTP_SW_PATCH_VERSION             26U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRTP_VERSION_H */
/*==================[end of file]============================================*/
