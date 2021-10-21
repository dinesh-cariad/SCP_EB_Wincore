#ifndef DLT_VERSION_H
#define DLT_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Dlt
 *
 * This file contains the implementation of the AUTOSAR
 * module Dlt.
 *
 * \version 1.8.6
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

#if (defined DLT_VENDOR_ID)
#error DLT_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define DLT_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined DLT_MODULE_ID)
#error DLT_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define DLT_MODULE_ID         55U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined DLT_AR_RELEASE_MAJOR_VERSION)
#error DLT_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define DLT_AR_RELEASE_MAJOR_VERSION     4U

#if (defined DLT_AR_RELEASE_MINOR_VERSION)
#error DLT_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define DLT_AR_RELEASE_MINOR_VERSION     2U

#if (defined DLT_AR_RELEASE_REVISION_VERSION)
#error DLT_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define DLT_AR_RELEASE_REVISION_VERSION  1U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined DLT_SW_MAJOR_VERSION)
#error DLT_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define DLT_SW_MAJOR_VERSION             1U

#if (defined DLT_SW_MINOR_VERSION)
#error DLT_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define DLT_SW_MINOR_VERSION             8U

#if (defined DLT_SW_PATCH_VERSION)
#error DLT_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define DLT_SW_PATCH_VERSION             6U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef DLT_VERSION_H */
/*==================[end of file]============================================*/
