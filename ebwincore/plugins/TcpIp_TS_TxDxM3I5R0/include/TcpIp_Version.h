#ifndef TCPIP_VERSION_H
#define TCPIP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR TcpIp
 *
 * This file contains the implementation of the AUTOSAR
 * module TcpIp.
 *
 * \version 3.5.13
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/


#include <TcpIp_Version.guards>
/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined TCPIP_VENDOR_ID)
#error TCPIP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define TCPIP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined TCPIP_MODULE_ID)
#error TCPIP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define TCPIP_MODULE_ID         170U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined TCPIP_AR_RELEASE_MAJOR_VERSION)
#error TCPIP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define TCPIP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined TCPIP_AR_RELEASE_MINOR_VERSION)
#error TCPIP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define TCPIP_AR_RELEASE_MINOR_VERSION     3U

#if (defined TCPIP_AR_RELEASE_REVISION_VERSION)
#error TCPIP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define TCPIP_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined TCPIP_SW_MAJOR_VERSION)
#error TCPIP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define TCPIP_SW_MAJOR_VERSION             3U

#if (defined TCPIP_SW_MINOR_VERSION)
#error TCPIP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define TCPIP_SW_MINOR_VERSION             5U

#if (defined TCPIP_SW_PATCH_VERSION)
#error TCPIP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define TCPIP_SW_PATCH_VERSION             13U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef TCPIP_VERSION_H */
/*==================[end of file]============================================*/
