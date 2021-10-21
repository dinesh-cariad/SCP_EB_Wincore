#ifndef STBM_VERSION_H
#define STBM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR StbM
 *
 * This file contains the implementation of the AUTOSAR
 * module StbM.
 *
 * \version 3.1.7
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

#if (defined STBM_VENDOR_ID)
#error STBM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define STBM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined STBM_MODULE_ID)
#error STBM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define STBM_MODULE_ID         160U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined STBM_AR_RELEASE_MAJOR_VERSION)
#error STBM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define STBM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined STBM_AR_RELEASE_MINOR_VERSION)
#error STBM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define STBM_AR_RELEASE_MINOR_VERSION     4U

#if (defined STBM_AR_RELEASE_REVISION_VERSION)
#error STBM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define STBM_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined STBM_SW_MAJOR_VERSION)
#error STBM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define STBM_SW_MAJOR_VERSION             3U

#if (defined STBM_SW_MINOR_VERSION)
#error STBM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define STBM_SW_MINOR_VERSION             1U

#if (defined STBM_SW_PATCH_VERSION)
#error STBM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define STBM_SW_PATCH_VERSION             7U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef STBM_VERSION_H */
/*==================[end of file]============================================*/
