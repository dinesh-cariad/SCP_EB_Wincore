#ifndef DET_VERSION_H
#define DET_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Det
 *
 * This file contains the implementation of the AUTOSAR
 * module Det.
 *
 * \version 6.5.6
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

#if (defined DET_VENDOR_ID)
#error DET_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define DET_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined DET_MODULE_ID)
#error DET_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define DET_MODULE_ID         15U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined DET_AR_RELEASE_MAJOR_VERSION)
#error DET_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define DET_AR_RELEASE_MAJOR_VERSION     4U

#if (defined DET_AR_RELEASE_MINOR_VERSION)
#error DET_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define DET_AR_RELEASE_MINOR_VERSION     0U

#if (defined DET_AR_RELEASE_REVISION_VERSION)
#error DET_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define DET_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined DET_SW_MAJOR_VERSION)
#error DET_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define DET_SW_MAJOR_VERSION             6U

#if (defined DET_SW_MINOR_VERSION)
#error DET_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define DET_SW_MINOR_VERSION             5U

#if (defined DET_SW_PATCH_VERSION)
#error DET_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define DET_SW_PATCH_VERSION             6U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef DET_VERSION_H */
/*==================[end of file]============================================*/
