/* !LINKSTO MemIf64,1 */
#ifndef MEMIF_VERSION_H
#define MEMIF_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR MemIf
 *
 * This file contains the implementation of the AUTOSAR
 * module MemIf.
 *
 * \version 5.11.11
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

#if (defined MEMIF_VENDOR_ID)
#error MEMIF_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define MEMIF_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined MEMIF_MODULE_ID)
#error MEMIF_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define MEMIF_MODULE_ID         22U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined MEMIF_AR_RELEASE_MAJOR_VERSION)
#error MEMIF_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define MEMIF_AR_RELEASE_MAJOR_VERSION     4U

#if (defined MEMIF_AR_RELEASE_MINOR_VERSION)
#error MEMIF_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define MEMIF_AR_RELEASE_MINOR_VERSION     0U

#if (defined MEMIF_AR_RELEASE_REVISION_VERSION)
#error MEMIF_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define MEMIF_AR_RELEASE_REVISION_VERSION  3U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined MEMIF_SW_MAJOR_VERSION)
#error MEMIF_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define MEMIF_SW_MAJOR_VERSION             5U

#if (defined MEMIF_SW_MINOR_VERSION)
#error MEMIF_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define MEMIF_SW_MINOR_VERSION             11U

#if (defined MEMIF_SW_PATCH_VERSION)
#error MEMIF_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define MEMIF_SW_PATCH_VERSION             11U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef MEMIF_VERSION_H */
/*==================[end of file]============================================*/
