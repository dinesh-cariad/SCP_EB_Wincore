#ifndef SOMEIPTP_VERSION_H
#define SOMEIPTP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR SomeIpTp
 *
 * This file contains the implementation of the AUTOSAR
 * module SomeIpTp.
 *
 * \version 1.0.28
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

#if (defined SOMEIPTP_VENDOR_ID)
#error SOMEIPTP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define SOMEIPTP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined SOMEIPTP_MODULE_ID)
#error SOMEIPTP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define SOMEIPTP_MODULE_ID         177U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined SOMEIPTP_AR_RELEASE_MAJOR_VERSION)
#error SOMEIPTP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define SOMEIPTP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined SOMEIPTP_AR_RELEASE_MINOR_VERSION)
#error SOMEIPTP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define SOMEIPTP_AR_RELEASE_MINOR_VERSION     3U

#if (defined SOMEIPTP_AR_RELEASE_REVISION_VERSION)
#error SOMEIPTP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define SOMEIPTP_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined SOMEIPTP_SW_MAJOR_VERSION)
#error SOMEIPTP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define SOMEIPTP_SW_MAJOR_VERSION             1U

#if (defined SOMEIPTP_SW_MINOR_VERSION)
#error SOMEIPTP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define SOMEIPTP_SW_MINOR_VERSION             0U

#if (defined SOMEIPTP_SW_PATCH_VERSION)
#error SOMEIPTP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define SOMEIPTP_SW_PATCH_VERSION             28U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef SOMEIPTP_VERSION_H */
/*==================[end of file]============================================*/
