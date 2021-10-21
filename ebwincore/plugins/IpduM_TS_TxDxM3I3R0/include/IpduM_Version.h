#ifndef IPDUM_VERSION_H
#define IPDUM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR IpduM
 *
 * This file contains the implementation of the AUTOSAR
 * module IpduM.
 *
 * \version 3.3.40
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

#if (defined IPDUM_VENDOR_ID)
#error IPDUM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define IPDUM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined IPDUM_MODULE_ID)
#error IPDUM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define IPDUM_MODULE_ID         52U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined IPDUM_AR_RELEASE_MAJOR_VERSION)
#error IPDUM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define IPDUM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined IPDUM_AR_RELEASE_MINOR_VERSION)
#error IPDUM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define IPDUM_AR_RELEASE_MINOR_VERSION     0U

#if (defined IPDUM_AR_RELEASE_REVISION_VERSION)
#error IPDUM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define IPDUM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined IPDUM_SW_MAJOR_VERSION)
#error IPDUM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define IPDUM_SW_MAJOR_VERSION             3U

#if (defined IPDUM_SW_MINOR_VERSION)
#error IPDUM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define IPDUM_SW_MINOR_VERSION             3U

#if (defined IPDUM_SW_PATCH_VERSION)
#error IPDUM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define IPDUM_SW_PATCH_VERSION             40U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef IPDUM_VERSION_H */
/*==================[end of file]============================================*/

