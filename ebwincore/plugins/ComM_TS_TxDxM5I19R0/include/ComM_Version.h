#ifndef COMM_VERSION_H
#define COMM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR ComM
 *
 * This file contains the implementation of the AUTOSAR
 * module ComM.
 *
 * \version 5.19.7
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

#if (defined COMM_VENDOR_ID)
#error COMM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define COMM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined COMM_MODULE_ID)
#error COMM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define COMM_MODULE_ID         12U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined COMM_AR_RELEASE_MAJOR_VERSION)
#error COMM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define COMM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined COMM_AR_RELEASE_MINOR_VERSION)
#error COMM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define COMM_AR_RELEASE_MINOR_VERSION     1U

#if (defined COMM_AR_RELEASE_REVISION_VERSION)
#error COMM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define COMM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined COMM_SW_MAJOR_VERSION)
#error COMM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define COMM_SW_MAJOR_VERSION             5U

#if (defined COMM_SW_MINOR_VERSION)
#error COMM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define COMM_SW_MINOR_VERSION             19U

#if (defined COMM_SW_PATCH_VERSION)
#error COMM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define COMM_SW_PATCH_VERSION             7U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef COMM_VERSION_H */
/*==================[end of file]============================================*/
