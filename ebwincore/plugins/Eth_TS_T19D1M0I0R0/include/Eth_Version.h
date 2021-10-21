#ifndef ETH_VERSION_H
#define ETH_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Eth
 *
 * This file contains the implementation of the AUTOSAR
 * module Eth.
 *
 * \version 0.0.8
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2011 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/

/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined ETH_VENDOR_ID)
#error ETH_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: EB Pcap Win32 driver */
#define ETH_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined ETH_MODULE_ID)
#error ETH_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define ETH_MODULE_ID         88U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ETH_AR_RELEASE_MAJOR_VERSION)
#error ETH_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ETH_AR_RELEASE_MAJOR_VERSION     4U

#if (defined ETH_AR_RELEASE_MINOR_VERSION)
#error ETH_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ETH_AR_RELEASE_MINOR_VERSION     1U

#if (defined ETH_AR_RELEASE_REVISION_VERSION)
#error ETH_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define ETH_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined ETH_SW_MAJOR_VERSION)
#error ETH_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define ETH_SW_MAJOR_VERSION             0U

#if (defined ETH_SW_MINOR_VERSION)
#error ETH_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define ETH_SW_MINOR_VERSION             0U

#if (defined ETH_SW_PATCH_VERSION)
#error ETH_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define ETH_SW_PATCH_VERSION             8U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef ETH_VERSION_H */
/*==================[end of file]============================================*/

