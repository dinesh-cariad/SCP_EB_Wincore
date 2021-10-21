#ifndef FRSM_VERSION_H
#define FRSM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR FrSM
 *
 * This file contains the implementation of the AUTOSAR
 * module FrSM.
 *
 * \version 5.3.18
 *
 * \author Elektrobit Automotive GmbH, 91058 Erlangen, Germany
 *
 * Copyright 2005 - 2021 Elektrobit Automotive GmbH
 * All rights exclusively reserved for Elektrobit Automotive GmbH,
 * unless expressly agreed to otherwise.
 */

/*==================[inclusions]=============================================*/


#include <FrSM_Version.guards>
/*==================[macros]=================================================*/

/*------------------[AUTOSAR vendor identification]-------------------------*/

#if (defined FRSM_VENDOR_ID)
#error FRSM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define FRSM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined FRSM_MODULE_ID)
#error FRSM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define FRSM_MODULE_ID         142U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined FRSM_AR_RELEASE_MAJOR_VERSION)
#error FRSM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define FRSM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined FRSM_AR_RELEASE_MINOR_VERSION)
#error FRSM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define FRSM_AR_RELEASE_MINOR_VERSION     0U

#if (defined FRSM_AR_RELEASE_REVISION_VERSION)
#error FRSM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define FRSM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined FRSM_SW_MAJOR_VERSION)
#error FRSM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define FRSM_SW_MAJOR_VERSION             5U

#if (defined FRSM_SW_MINOR_VERSION)
#error FRSM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define FRSM_SW_MINOR_VERSION             3U

#if (defined FRSM_SW_PATCH_VERSION)
#error FRSM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define FRSM_SW_PATCH_VERSION             18U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef FRSM_VERSION_H */
/*==================[end of file]============================================*/
