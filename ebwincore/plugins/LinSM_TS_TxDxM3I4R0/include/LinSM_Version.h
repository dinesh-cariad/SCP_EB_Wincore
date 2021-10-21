/* !LINKSTO LinSM.ASR40.LINSM210, 1 */
#ifndef LINSM_VERSION_H
#define LINSM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR LinSM
 *
 * This file contains the implementation of the AUTOSAR
 * module LinSM.
 *
 * \version 3.4.19
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

#if (defined LINSM_VENDOR_ID)
#error LINSM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define LINSM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined LINSM_MODULE_ID)
#error LINSM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define LINSM_MODULE_ID         141U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined LINSM_AR_RELEASE_MAJOR_VERSION)
#error LINSM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define LINSM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined LINSM_AR_RELEASE_MINOR_VERSION)
#error LINSM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define LINSM_AR_RELEASE_MINOR_VERSION     0U

#if (defined LINSM_AR_RELEASE_REVISION_VERSION)
#error LINSM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define LINSM_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined LINSM_SW_MAJOR_VERSION)
#error LINSM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define LINSM_SW_MAJOR_VERSION             3U

#if (defined LINSM_SW_MINOR_VERSION)
#error LINSM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define LINSM_SW_MINOR_VERSION             4U

#if (defined LINSM_SW_PATCH_VERSION)
#error LINSM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define LINSM_SW_PATCH_VERSION             19U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef LINSM_VERSION_H */
/*==================[end of file]============================================*/
