#ifndef XCP_VERSION_H
#define XCP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Xcp
 *
 * This file contains the implementation of the AUTOSAR
 * module Xcp.
 *
 * \version 2.12.0
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

#if (defined XCP_VENDOR_ID)
#error XCP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define XCP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined XCP_MODULE_ID)
#error XCP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define XCP_MODULE_ID         212U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined XCP_AR_RELEASE_MAJOR_VERSION)
#error XCP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define XCP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined XCP_AR_RELEASE_MINOR_VERSION)
#error XCP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define XCP_AR_RELEASE_MINOR_VERSION     0U

#if (defined XCP_AR_RELEASE_REVISION_VERSION)
#error XCP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define XCP_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined XCP_SW_MAJOR_VERSION)
#error XCP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define XCP_SW_MAJOR_VERSION             2U

#if (defined XCP_SW_MINOR_VERSION)
#error XCP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define XCP_SW_MINOR_VERSION             12U

#if (defined XCP_SW_PATCH_VERSION)
#error XCP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define XCP_SW_PATCH_VERSION             0U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef XCP_VERSION_H */
/*==================[end of file]============================================*/
