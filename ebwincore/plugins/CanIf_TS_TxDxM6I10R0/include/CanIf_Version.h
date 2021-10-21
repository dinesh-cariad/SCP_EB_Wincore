#ifndef CANIF_VERSION_H
#define CANIF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR CanIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CanIf.
 *
 * \version 6.10.15
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

#if (defined CANIF_VENDOR_ID)
#error CANIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CANIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CANIF_MODULE_ID)
#error CANIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define CANIF_MODULE_ID         60U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined CANIF_AR_RELEASE_MAJOR_VERSION)
#error CANIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define CANIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined CANIF_AR_RELEASE_MINOR_VERSION)
#error CANIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define CANIF_AR_RELEASE_MINOR_VERSION     0U

#if (defined CANIF_AR_RELEASE_REVISION_VERSION)
#error CANIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define CANIF_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CANIF_SW_MAJOR_VERSION)
#error CANIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define CANIF_SW_MAJOR_VERSION             6U

#if (defined CANIF_SW_MINOR_VERSION)
#error CANIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define CANIF_SW_MINOR_VERSION             10U

#if (defined CANIF_SW_PATCH_VERSION)
#error CANIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define CANIF_SW_PATCH_VERSION             15U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CANIF_VERSION_H */
/*==================[end of file]============================================*/
