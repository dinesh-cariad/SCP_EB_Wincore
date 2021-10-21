/* !LINKSTO EthIf.ASR43.SWS_EthIf_00006,1 */
#ifndef ETHIF_VERSION_H
#define ETHIF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR EthIf
 *
 * This file contains the implementation of the AUTOSAR
 * module EthIf.
 *
 * \version 1.9.18
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

#if (defined ETHIF_VENDOR_ID)
#error ETHIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define ETHIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined ETHIF_MODULE_ID)
#error ETHIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define ETHIF_MODULE_ID         65U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ETHIF_AR_RELEASE_MAJOR_VERSION)
#error ETHIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ETHIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined ETHIF_AR_RELEASE_MINOR_VERSION)
#error ETHIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ETHIF_AR_RELEASE_MINOR_VERSION     3U

#if (defined ETHIF_AR_RELEASE_REVISION_VERSION)
#error ETHIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define ETHIF_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined ETHIF_SW_MAJOR_VERSION)
#error ETHIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define ETHIF_SW_MAJOR_VERSION             1U

#if (defined ETHIF_SW_MINOR_VERSION)
#error ETHIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define ETHIF_SW_MINOR_VERSION             9U

#if (defined ETHIF_SW_PATCH_VERSION)
#error ETHIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define ETHIF_SW_PATCH_VERSION             18U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef ETHIF_VERSION_H */
/*==================[end of file]============================================*/
