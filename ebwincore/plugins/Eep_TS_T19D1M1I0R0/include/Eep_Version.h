#ifndef EEP_VERSION_H
#define EEP_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Eep
 *
 * This file contains the implementation of the AUTOSAR
 * module Eep.
 *
 * \version 1.0.9
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

#if (defined EEP_VENDOR_ID)
#error EEP_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define EEP_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined EEP_MODULE_ID)
#error EEP_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define EEP_MODULE_ID         90U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined EEP_AR_RELEASE_MAJOR_VERSION)
#error EEP_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define EEP_AR_RELEASE_MAJOR_VERSION     4U

#if (defined EEP_AR_RELEASE_MINOR_VERSION)
#error EEP_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define EEP_AR_RELEASE_MINOR_VERSION     0U

#if (defined EEP_AR_RELEASE_REVISION_VERSION)
#error EEP_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define EEP_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined EEP_SW_MAJOR_VERSION)
#error EEP_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define EEP_SW_MAJOR_VERSION             1U

#if (defined EEP_SW_MINOR_VERSION)
#error EEP_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define EEP_SW_MINOR_VERSION             0U

#if (defined EEP_SW_PATCH_VERSION)
#error EEP_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define EEP_SW_PATCH_VERSION             9U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef EEP_VERSION_H */
/*==================[end of file]============================================*/
