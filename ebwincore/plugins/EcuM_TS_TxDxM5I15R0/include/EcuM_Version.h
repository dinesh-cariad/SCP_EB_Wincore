#ifndef ECUM_VERSION_H
#define ECUM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR EcuM
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuM.
 *
 * \version 5.15.7
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

#if (defined ECUM_VENDOR_ID)
#error ECUM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define ECUM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined ECUM_MODULE_ID)
#error ECUM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define ECUM_MODULE_ID         10U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ECUM_AR_RELEASE_MAJOR_VERSION)
#error ECUM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ECUM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined ECUM_AR_RELEASE_MINOR_VERSION)
#error ECUM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ECUM_AR_RELEASE_MINOR_VERSION     0U

#if (defined ECUM_AR_RELEASE_REVISION_VERSION)
#error ECUM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define ECUM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined ECUM_SW_MAJOR_VERSION)
#error ECUM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define ECUM_SW_MAJOR_VERSION             5U

#if (defined ECUM_SW_MINOR_VERSION)
#error ECUM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define ECUM_SW_MINOR_VERSION             15U

#if (defined ECUM_SW_PATCH_VERSION)
#error ECUM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define ECUM_SW_PATCH_VERSION             7U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef ECUM_VERSION_H */
/*==================[end of file]============================================*/
