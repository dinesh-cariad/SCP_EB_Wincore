#ifndef DCM_VERSION_H
#define DCM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Dcm
 *
 * This file contains the implementation of the AUTOSAR
 * module Dcm.
 *
 * \version 5.0.9
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

#if (defined DCM_VENDOR_ID)
#error DCM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define DCM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined DCM_MODULE_ID)
#error DCM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define DCM_MODULE_ID         53U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined DCM_AR_RELEASE_MAJOR_VERSION)
#error DCM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define DCM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined DCM_AR_RELEASE_MINOR_VERSION)
#error DCM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define DCM_AR_RELEASE_MINOR_VERSION     0U

#if (defined DCM_AR_RELEASE_REVISION_VERSION)
#error DCM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define DCM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined DCM_SW_MAJOR_VERSION)
#error DCM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define DCM_SW_MAJOR_VERSION             5U

#if (defined DCM_SW_MINOR_VERSION)
#error DCM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define DCM_SW_MINOR_VERSION             0U

#if (defined DCM_SW_PATCH_VERSION)
#error DCM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define DCM_SW_PATCH_VERSION             9U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef DCM_VERSION_H */
/*==================[end of file]============================================*/
