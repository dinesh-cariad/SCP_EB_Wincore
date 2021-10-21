#ifndef KEYM_VERSION_H
#define KEYM_VERSION_H
/**
 * \file
 *
 * \brief AUTOSAR KeyM
 *
 * This file contains the implementation of the AUTOSAR
 * module KeyM.
 *
 * \version 1.2.11
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

#if (defined KEYM_VENDOR_ID)
#error KEYM_VENDOR_ID is already defined
#endif
/** \brief AUTOSAR vendor identification: Elektrobit Automotive GmbH */
#define KEYM_VENDOR_ID         1U

/*------------------[AUTOSAR module identification]-------------------------*/

#if (defined KEYM_MODULE_ID)
#error KEYM_MODULE_ID already defined
#endif
/** \brief AUTOSAR module identification */
#define KEYM_MODULE_ID         109U

/*------------------[AUTOSAR release version identification]----------------*/

#if (defined KEYM_AR_RELEASE_MAJOR_VERSION)
#error KEYM_AR_RELEASE_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR release major version */
#define KEYM_AR_RELEASE_MAJOR_VERSION     4U

#if (defined KEYM_AR_RELEASE_MINOR_VERSION)
#error KEYM_AR_RELEASE_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR release minor version */
#define KEYM_AR_RELEASE_MINOR_VERSION     3U

#if (defined KEYM_AR_RELEASE_REVISION_VERSION)
#error KEYM_AR_RELEASE_REVISION_VERSION already defined
#endif
/** \brief AUTOSAR release revision version */
#define KEYM_AR_RELEASE_REVISION_VERSION  0U

/*------------------[AUTOSAR module version identification]------------------*/

#if (defined KEYM_SW_MAJOR_VERSION)
#error KEYM_SW_MAJOR_VERSION already defined
#endif
/** \brief AUTOSAR module major version */
#define KEYM_SW_MAJOR_VERSION             1U

#if (defined KEYM_SW_MINOR_VERSION)
#error KEYM_SW_MINOR_VERSION already defined
#endif
/** \brief AUTOSAR module minor version */
#define KEYM_SW_MINOR_VERSION             2U

#if (defined KEYM_SW_PATCH_VERSION)
#error KEYM_SW_PATCH_VERSION already defined
#endif
/** \brief AUTOSAR module patch version */
#define KEYM_SW_PATCH_VERSION             11U

/*==================[type definitions]=======================================*/

/*==================[external function declarations]=========================*/

/*==================[internal function declarations]=========================*/

/*==================[external constants]=====================================*/

/*==================[internal constants]=====================================*/

/*==================[external data]==========================================*/

/*==================[internal data]==========================================*/

/*==================[external function definitions]==========================*/

/*==================[internal function definitions]==========================*/

#endif /* ifndef KEYM_VERSION_H */
/*==================[end of file]============================================*/

