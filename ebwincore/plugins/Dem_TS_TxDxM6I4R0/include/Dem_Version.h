#ifndef DEM_VERSION_H
#define DEM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Dem
 *
 * This file contains the implementation of the AUTOSAR
 * module Dem.
 *
 * \version 6.4.1
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

#if (defined DEM_VENDOR_ID)
#error DEM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define DEM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined DEM_MODULE_ID)
#error DEM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define DEM_MODULE_ID         54U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined DEM_AR_RELEASE_MAJOR_VERSION)
#error DEM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define DEM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined DEM_AR_RELEASE_MINOR_VERSION)
#error DEM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define DEM_AR_RELEASE_MINOR_VERSION     0U

#if (defined DEM_AR_RELEASE_REVISION_VERSION)
#error DEM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define DEM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined DEM_SW_MAJOR_VERSION)
#error DEM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define DEM_SW_MAJOR_VERSION             6U

#if (defined DEM_SW_MINOR_VERSION)
#error DEM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define DEM_SW_MINOR_VERSION             4U

#if (defined DEM_SW_PATCH_VERSION)
#error DEM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define DEM_SW_PATCH_VERSION             1U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef DEM_VERSION_H */
/*==================[end of file]============================================*/
