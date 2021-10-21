#ifndef UDPNM_VERSION_H
#define UDPNM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR UdpNm
 *
 * This file contains the implementation of the AUTOSAR
 * module UdpNm.
 *
 * \version 2.9.7
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

#if (defined UDPNM_VENDOR_ID)
#error UDPNM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define UDPNM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined UDPNM_MODULE_ID)
#error UDPNM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define UDPNM_MODULE_ID         33U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined UDPNM_AR_RELEASE_MAJOR_VERSION)
#error UDPNM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define UDPNM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined UDPNM_AR_RELEASE_MINOR_VERSION)
#error UDPNM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define UDPNM_AR_RELEASE_MINOR_VERSION     1U

#if (defined UDPNM_AR_RELEASE_REVISION_VERSION)
#error UDPNM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define UDPNM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined UDPNM_SW_MAJOR_VERSION)
#error UDPNM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define UDPNM_SW_MAJOR_VERSION             2U

#if (defined UDPNM_SW_MINOR_VERSION)
#error UDPNM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define UDPNM_SW_MINOR_VERSION             9U

#if (defined UDPNM_SW_PATCH_VERSION)
#error UDPNM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define UDPNM_SW_PATCH_VERSION             7U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef UDPNM_VERSION_H */
/*==================[end of file]============================================*/
