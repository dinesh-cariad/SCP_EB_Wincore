#ifndef NM_VERSION_H
#define NM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR Nm
 *
 * This file contains the implementation of the AUTOSAR
 * module Nm.
 *
 * \version 5.12.8
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

#if (defined NM_VENDOR_ID)
#error NM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define NM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined NM_MODULE_ID)
#error NM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define NM_MODULE_ID         29U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined NM_AR_RELEASE_MAJOR_VERSION)
#error NM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define NM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined NM_AR_RELEASE_MINOR_VERSION)
#error NM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define NM_AR_RELEASE_MINOR_VERSION     0U

#if (defined NM_AR_RELEASE_REVISION_VERSION)
#error NM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define NM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined NM_SW_MAJOR_VERSION)
#error NM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define NM_SW_MAJOR_VERSION             5U

#if (defined NM_SW_MINOR_VERSION)
#error NM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define NM_SW_MINOR_VERSION             12U

#if (defined NM_SW_PATCH_VERSION)
#error NM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define NM_SW_PATCH_VERSION             8U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef NM_VERSION_H */
/*==================[end of file]============================================*/
