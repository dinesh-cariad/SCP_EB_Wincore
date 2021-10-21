#ifndef PDUR_VERSION_H
#define PDUR_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR PduR
 *
 * This file contains the implementation of the AUTOSAR
 * module PduR.
 *
 * \version 5.3.46
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

#if (defined PDUR_VENDOR_ID)
#error PDUR_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define PDUR_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined PDUR_MODULE_ID)
#error PDUR_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define PDUR_MODULE_ID         51U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined PDUR_AR_RELEASE_MAJOR_VERSION)
#error PDUR_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define PDUR_AR_RELEASE_MAJOR_VERSION     4U

#if (defined PDUR_AR_RELEASE_MINOR_VERSION)
#error PDUR_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define PDUR_AR_RELEASE_MINOR_VERSION     0U

#if (defined PDUR_AR_RELEASE_REVISION_VERSION)
#error PDUR_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define PDUR_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined PDUR_SW_MAJOR_VERSION)
#error PDUR_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define PDUR_SW_MAJOR_VERSION             5U

#if (defined PDUR_SW_MINOR_VERSION)
#error PDUR_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define PDUR_SW_MINOR_VERSION             3U

#if (defined PDUR_SW_PATCH_VERSION)
#error PDUR_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define PDUR_SW_PATCH_VERSION             46U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef PDUR_VERSION_H */
/*==================[end of file]============================================*/

