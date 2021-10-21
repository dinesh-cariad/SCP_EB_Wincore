#ifndef CRYIF_VERSION_H
#define CRYIF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR CryIf
 *
 * This file contains the implementation of the AUTOSAR
 * module CryIf.
 *
 * \version 1.0.24
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

#if (defined CRYIF_VENDOR_ID)
#error CRYIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define CRYIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined CRYIF_MODULE_ID)
#error CRYIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define CRYIF_MODULE_ID         112U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined CRYIF_AR_RELEASE_MAJOR_VERSION)
#error CRYIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define CRYIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined CRYIF_AR_RELEASE_MINOR_VERSION)
#error CRYIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define CRYIF_AR_RELEASE_MINOR_VERSION     3U

#if (defined CRYIF_AR_RELEASE_REVISION_VERSION)
#error CRYIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define CRYIF_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined CRYIF_SW_MAJOR_VERSION)
#error CRYIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define CRYIF_SW_MAJOR_VERSION             1U

#if (defined CRYIF_SW_MINOR_VERSION)
#error CRYIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define CRYIF_SW_MINOR_VERSION             0U

#if (defined CRYIF_SW_PATCH_VERSION)
#error CRYIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define CRYIF_SW_PATCH_VERSION             24U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef CRYIF_VERSION_H */
/*==================[end of file]============================================*/

