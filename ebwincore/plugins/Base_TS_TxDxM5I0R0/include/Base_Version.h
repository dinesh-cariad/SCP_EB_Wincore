#ifndef BASE_VERSION_H
#define BASE_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Base
 *
 * This file contains the implementation of the AUTOSAR
 * module Base.
 *
 * \version 5.0.31
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

#if (defined BASE_VENDOR_ID)
#error BASE_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define BASE_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined BASE_MODULE_ID)
#error BASE_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define BASE_MODULE_ID         0U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined BASE_AR_RELEASE_MAJOR_VERSION)
#error BASE_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define BASE_AR_RELEASE_MAJOR_VERSION     4U

#if (defined BASE_AR_RELEASE_MINOR_VERSION)
#error BASE_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define BASE_AR_RELEASE_MINOR_VERSION     0U

#if (defined BASE_AR_RELEASE_REVISION_VERSION)
#error BASE_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define BASE_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined BASE_SW_MAJOR_VERSION)
#error BASE_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define BASE_SW_MAJOR_VERSION             5U

#if (defined BASE_SW_MINOR_VERSION)
#error BASE_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define BASE_SW_MINOR_VERSION             0U

#if (defined BASE_SW_PATCH_VERSION)
#error BASE_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define BASE_SW_PATCH_VERSION             31U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef BASE_VERSION_H */
/*==================[end of file]============================================*/
