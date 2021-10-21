#ifndef ECUC_VERSION_H
#define ECUC_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR EcuC
 *
 * This file contains the implementation of the AUTOSAR
 * module EcuC.
 *
 * \version 5.0.21
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

#if (defined ECUC_VENDOR_ID)
#error ECUC_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define ECUC_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined ECUC_MODULE_ID)
#error ECUC_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define ECUC_MODULE_ID         10U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined ECUC_AR_RELEASE_MAJOR_VERSION)
#error ECUC_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define ECUC_AR_RELEASE_MAJOR_VERSION     4U

#if (defined ECUC_AR_RELEASE_MINOR_VERSION)
#error ECUC_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define ECUC_AR_RELEASE_MINOR_VERSION     0U

#if (defined ECUC_AR_RELEASE_REVISION_VERSION)
#error ECUC_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define ECUC_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined ECUC_SW_MAJOR_VERSION)
#error ECUC_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define ECUC_SW_MAJOR_VERSION             5U

#if (defined ECUC_SW_MINOR_VERSION)
#error ECUC_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define ECUC_SW_MINOR_VERSION             0U

#if (defined ECUC_SW_PATCH_VERSION)
#error ECUC_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define ECUC_SW_PATCH_VERSION             21U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef ECUC_VERSION_H */
/*==================[end of file]============================================*/
