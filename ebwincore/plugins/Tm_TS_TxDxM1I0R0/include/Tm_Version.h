#ifndef TM_VERSION_H
#define TM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Tm
 *
 * This file contains the implementation of the AUTOSAR
 * module Tm.
 *
 * \version 1.0.4
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

#if (defined TM_VENDOR_ID)
#error TM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define TM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined TM_MODULE_ID)
#error TM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define TM_MODULE_ID         014U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined TM_AR_RELEASE_MAJOR_VERSION)
#error TM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define TM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined TM_AR_RELEASE_MINOR_VERSION)
#error TM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define TM_AR_RELEASE_MINOR_VERSION     4U

#if (defined TM_AR_RELEASE_REVISION_VERSION)
#error TM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define TM_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined TM_SW_MAJOR_VERSION)
#error TM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define TM_SW_MAJOR_VERSION             1U

#if (defined TM_SW_MINOR_VERSION)
#error TM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define TM_SW_MINOR_VERSION             0U

#if (defined TM_SW_PATCH_VERSION)
#error TM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define TM_SW_PATCH_VERSION             4U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef TM_VERSION_H */
/*==================[end of file]============================================*/
